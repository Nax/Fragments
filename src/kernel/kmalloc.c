#include "kernel.h"

#define KERNEL_HEAP_BASE    0xffffffffa0000000

static size_t _roundPagesize(size_t size)
{
    if (size == 0)
        return 0;
    return (((size - 1) / PAGESIZE) + 1) * PAGESIZE;
}

void* kheap_alloc(size_t size)
{
    size_t fixedSize;
    uint64_t vaddr;

    if (size == 0)
        return NULL;
    fixedSize = _roundPagesize(size);
    vaddr = KERNEL_HEAP_BASE + gKernel.heapSize;
    gKernel.heapSize += fixedSize;

    return (void*)vaddr;
}

static void* _heapAllocBacked(size_t size)
{
    void* vaddr;
    size_t pageCount;
    page_addr page;

    vaddr = kheap_alloc(size);
    pageCount = _roundPagesize(size) / PAGESIZE;

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

    heapSize = _roundPagesize(size + sizeof(KernelBlockHeader));
    block = ((KernelBlockHeader*)_heapAllocBacked(heapSize)) + 1;
    block[-1].size = (uint32_t)heapSize;

    return block;
}

void* kmalloc(int flags, size_t size)
{
    void* block;

    block = _allocBlock(size);
    return block;
}
