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

#include "stage2.h"

void detect_memory(FragmentsKernelInfo* info)
{
    int carry;
    Regs r;
    FragmentsMemRegion mem;

    info->memRegionCount = 0;
    println("===== MEMORY MAP =====");
    r.ebx = 0;
    for (;;)
    {
        mem.extended = 1;
        r.eax = 0xe820;
        r.ecx = 24;
        r.edx = 0x534d4150;
        r.edi = (uint32_t)&mem;
        carry = bios(0x15, &r);
        if (carry || r.ebx == 0)
            break;
        if (mem.size == 0)
            continue;
        print("Mem Region: ");
        printhex64(mem.base);
        putchar(' ');
        printhex64(mem.size);
        putchar(' ');
        printhex32(mem.type);
        putchar('\n');

        memcpy(info->memRegions + info->memRegionCount, &mem, sizeof(mem));
        info->memRegionCount++;
    }
    println("======================\n");

    for (size_t i = 0; i < info->memRegionCount; ++i)
    {
        FragmentsMemRegion* region;

        region = &info->memRegions[i];
        if (region->type == 1 && region->base >= 0x100000 && region->size >= 0x100000)
        {
            info->pageUseBase = region->base;
            info->pageUseSize = 0;
            break;
        }
    }
}

void* alloc_pages(FragmentsKernelInfo* info, size_t size)
{
    uint64_t page;
    uint64_t actualSize;

    actualSize = (((size - 1) / PAGESIZE) + 1) * PAGESIZE;

    page = info->pageUseBase + info->pageUseSize;
    info->pageUseSize += actualSize;

    return (void*)((uint32_t)page);
}
