/*
 * Copyright (c) 2019, Maxime Bacoux
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
    page = allocator->stack[--allocator->count];

    return page;
}

void pmem_init(void)
{
    PageAllocator* allocator;
    page_addr page;

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
