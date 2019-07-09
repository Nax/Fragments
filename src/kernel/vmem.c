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
