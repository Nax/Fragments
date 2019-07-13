#include <string.h>
#include "kernel.h"

#define KERNEL_HEAP_BASE    0xffffffffa0000000
#define KERNEL_HEAP_MASK    0xffffffff00000000

void* kheap_alloc(size_t size)
{
    size_t fixedSize;
    uint64_t vaddr;

    if (size == 0)
        return NULL;
    fixedSize = page_size_round(size);
    vaddr = KERNEL_HEAP_BASE + gKernel.heapSize;
    gKernel.heapSize += fixedSize + PAGESIZE;

    return (void*)vaddr;
}

void* kmalloc_raw(size_t size)
{
    void* vaddr;
    size_t pageCount;
    page_addr page;

    vaddr = kheap_alloc(size);
    pageCount = page_size_round(size) / PAGESIZE;

    for (size_t i = 0; i < pageCount; ++i)
    {
        page = pmem_alloc_page();
        vmem_map((char*)vaddr + i * PAGESIZE, page, 0);
    }

    return vaddr;
}

static void* _allocBlock(size_t size)
{
    size_t heapSize;
    KernelBlockHeader* block;

    heapSize = page_size_round(size + sizeof(KernelBlockHeader));
    block = ((KernelBlockHeader*)kmalloc_raw(heapSize)) + 1;
    block[-1].size = (uint32_t)heapSize;

    return block;
}

/*
static void _dumpFreeBlocks(void)
{
    print("malloc free block stack: ");
    printhex64(gKernel.memoryAllocator.freeBlocks);
    putchar('\n');
    print("===== FREE BLOCKS =====\n");
    for (size_t i = 0; i < gKernel.memoryAllocator.freeBlockCount; ++i)
    {
        printhex8((uint8_t)i);
        print(": ");
        printhex32(gKernel.memoryAllocator.freeBlocks[i].size);
        print(" ");
        printhex32(gKernel.memoryAllocator.freeBlocks[i].offset);
        putchar('\n');
    }
    print("=======================\n\n");
}
*/

void* kmalloc(int flags, size_t size)
{
    (void)flags;

    volatile KernelMemoryAllocator* allocator;
    void* block;
    size_t i;

    allocator = &gKernel.memoryAllocator;
    i = allocator->freeBlockCount;
    while (i--)
    {
        if (allocator->freeBlocks[i].size >= size + sizeof(KernelBlockHeader))
        {
            /* Found a free block */
            block = (void*)((uintptr_t)allocator->freeBlocks[i].offset | KERNEL_HEAP_MASK);
            memmove(allocator->freeBlocks + i, allocator->freeBlocks + i + 1, (allocator->freeBlockCount - i - 1) * sizeof(KernelFreeBlock));
            allocator->freeBlockCount--;
            return block;
        }
    }
    block = _allocBlock(size);
    return block;
}

void kfree(void* addr)
{
    size_t newCapacity;
    KernelFreeBlock* oldStack;
    KernelFreeBlock* newStack;

    volatile KernelMemoryAllocator* allocator;
    KernelBlockHeader* block;

    oldStack = NULL;

    allocator = &gKernel.memoryAllocator;
    block = (KernelBlockHeader*)addr;

    if (allocator->freeBlockCount == allocator->freeBlockCapacity)
    {
        if (!allocator->freeBlockCapacity)
            newCapacity = 128;
        else
            newCapacity = allocator->freeBlockCapacity + allocator->freeBlockCapacity / 2;
        oldStack = allocator->freeBlocks;
        newStack = kmalloc(0, newCapacity * sizeof(*allocator->freeBlocks));
        memcpy(newStack, oldStack, allocator->freeBlockCapacity * sizeof(*allocator->freeBlocks));
        allocator->freeBlockCapacity = newCapacity;
        allocator->freeBlocks = newStack;
    }

    allocator->freeBlocks[allocator->freeBlockCount].offset = (uintptr_t)addr & 0xffffffff;
    allocator->freeBlocks[allocator->freeBlockCount].size = block[-1].size;
    allocator->freeBlockCount++;

    if (oldStack)
        kfree(oldStack);
}
