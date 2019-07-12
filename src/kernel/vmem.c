#include <string.h>
#include "kernel.h"

#define PAGE_RECURSE    0x1fe
#define RECURSE_BASE    0xffffff0000000000

page_addr* _recursivePagePointer(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    uint64_t addr;

    addr = RECURSE_BASE;
    addr |= ((uint64_t)a << (3 + 9 * 3));
    addr |= ((uint64_t)b << (3 + 9 * 2));
    addr |= ((uint64_t)c << (3 + 9 * 1));
    addr |= ((uint64_t)d << (3 + 9 * 0));

    return (page_addr*)addr;
}

static void _vmem_ensure_mapped(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    page_addr* ptr;
    page_addr newPage;

    ptr = _recursivePagePointer(a, b, c, d);
    if (*ptr == 0)
    {
        newPage = pmem_alloc_page();
        *ptr = (newPage | 1);
        memset(_recursivePagePointer(b, c, d, 0), 0, PAGESIZE);
    }
}

void vmem_map(void* vaddr, page_addr page, int flags)
{
    (void)flags;

    uint16_t a;
    uint16_t b;
    uint16_t c;
    uint16_t d;
    page_addr* pageSlot;

    a = (((uint64_t)vaddr) >> (3 + 9 * 4)) & 0x1ff;
    b = (((uint64_t)vaddr) >> (3 + 9 * 3)) & 0x1ff;
    c = (((uint64_t)vaddr) >> (3 + 9 * 2)) & 0x1ff;
    d = (((uint64_t)vaddr) >> (3 + 9 * 1)) & 0x1ff;

    _vmem_ensure_mapped(PAGE_RECURSE, PAGE_RECURSE, PAGE_RECURSE, a);
    _vmem_ensure_mapped(PAGE_RECURSE, PAGE_RECURSE, a, b);
    _vmem_ensure_mapped(PAGE_RECURSE, a, b, c);

    pageSlot = _recursivePagePointer(a, b, c, d);
    *pageSlot = (page | 1);
}

void* vmem_io_map(page_addr base, size_t size)
{
    void* addr;

    size = page_size_round(size);
    addr = kheap_alloc(size);
    for (size_t i = 0; i < size / PAGESIZE; ++i)
        vmem_map((char*)addr + PAGESIZE * i, base + PAGESIZE * i, 0);
    return addr;
}

static void _freePageInMap(page_addr* addr)
{
    page_addr cleanPage;

    cleanPage = *addr & ~(0xfff);
    *addr = 0;
    pmem_free_page(cleanPage);
}

void vmem_unmap_lower(void)
{
    uint16_t a;
    uint16_t b;
    uint16_t c;
    uint16_t d;
    page_addr* pageA;
    page_addr* pageB;
    page_addr* pageC;
    page_addr* pageD;

    for (a = 0; a < 256; ++a)
    {
        pageA = _recursivePagePointer(PAGE_RECURSE, PAGE_RECURSE, PAGE_RECURSE, a);
        if (*pageA)
        {
            for (b = 0; b < 512; ++b)
            {
                pageB = _recursivePagePointer(PAGE_RECURSE, PAGE_RECURSE, a, b);
                if (*pageB)
                {
                    for (c = 0; c < 512; ++c)
                    {
                        pageC = _recursivePagePointer(PAGE_RECURSE, a, b, c);
                        if (*pageC)
                        {
                            for (d = 0; d < 512; ++d)
                            {
                                pageD = _recursivePagePointer(a, b, c, d);
                                if (*pageD)
                                    _freePageInMap(pageD);
                            }
                            _freePageInMap(pageC);
                        }
                    }
                    _freePageInMap(pageB);
                }
            }
            _freePageInMap(pageA);
        }
    }

    /* Force a TLB flush */
    __asm__ __volatile__ (
        "mov %%cr3, %%rax\r\n"
        "mov %%rax, %%cr3\r\n"
        ::: "rax"
    );
}
