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

void* alloc_page(FragmentsKernelInfo* info)
{
    uint64_t page;

    page = info->pageUseBase + info->pageUseSize;
    info->pageUseSize += 0x1000;

    return (void*)((uint32_t)page);
}
