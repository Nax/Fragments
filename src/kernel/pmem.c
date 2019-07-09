#include <string.h>
#include "kernel.h"

static page_addr _pmem_alloc_page_watermark(void)
{
    page_addr addr;

    addr = gKernel.bootInfo.pageUseBase + gKernel.bootInfo.pageUseSize;
    gKernel.bootInfo.pageUseSize += PAGESIZE;

    return addr;
}

static page_addr _pmem_alloc_page_stack(void)
{
    PageAllocator* allocator;
    page_addr page;

    allocator = &gKernel.pageAllocator;
    page = allocator->stack[allocator->count];
    allocator->count--;
    return page;
}

void pmem_init(void)
{
    PageAllocator* allocator;
    page_addr page;

    __asm__ __volatile__ ("xchg %bx, %bx\r\n");

    println("Test");

    allocator = &gKernel.pageAllocator;
    allocator->capacity = 500;
    allocator->stack = kmalloc(0, allocator->capacity * sizeof(page_addr));

    for (size_t zoneIndex = 0; zoneIndex < gKernel.bootInfo.memRegionCount; ++zoneIndex)
    {
        FragmentsMemRegion* zone;

        zone = gKernel.bootInfo.memRegions + zoneIndex;
        if (zone->type != 1)
            continue;
        for (size_t i = 0; i < zone->size / PAGESIZE; ++i)
        {
            page = zone->base + i * PAGESIZE;
            if (page >= gKernel.bootInfo.pageUseBase && page < gKernel.bootInfo.pageUseBase + gKernel.bootInfo.pageUseSize)
                continue;
            pmem_free_page(page);
        }
    }
    gKernel.bootInfo.pageUseBase = 0;
    gKernel.bootInfo.pageUseSize = 0;
    print("Pages available: ");
    printhex64(allocator->count);
    putchar('\n');
}

page_addr pmem_alloc_page(void)
{
    PageAllocator* allocator;

    allocator = &gKernel.pageAllocator;
    if (allocator->stack)
        return _pmem_alloc_page_stack();
    else
        return _pmem_alloc_page_watermark();
}

void pmem_free_page(page_addr page)
{
    PageAllocator* allocator;
    size_t newCapacity;
    page_addr* oldStack;
    page_addr* newStack;

    allocator = &gKernel.pageAllocator;
    page &= 0xfffffffffffff000;

    if (allocator->count == allocator->capacity)
    {
        newCapacity = allocator->capacity + allocator->capacity / 2;
        newStack = kmalloc(0, newCapacity * sizeof(page_addr));
        memcpy(newStack, allocator->stack, allocator->count * sizeof(page_addr));
        oldStack = allocator->stack;
        allocator->stack = newStack;
        allocator->capacity = newCapacity;
        kfree(oldStack);
    }

    allocator->stack[allocator->count++] = page;
}
