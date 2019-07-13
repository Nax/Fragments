#include <string.h>
#include "kernel.h"

void vmem_init(void)
{
    page_addr* pml4;
    page_addr* identity;

    pml4 = (page_addr*)gKernel.cr3;
    identity = (page_addr*)pmem_alloc_page();
    for (size_t i = 0; i < 512; ++i)
        identity[i] = (i << 30) | 0x81;
    pml4[PAGE_BASE_ENTRY] = (page_addr)identity | 1;
}

void* vmem_physical(page_addr page)
{
    return (void*)(PAGE_BASE | (page & 0xfffffffffffff000));
}

page_addr* _ensure_mapped(page_addr* table, uint16_t index)
{
    page_addr page;
    page_addr* tmp;

    page = table[index];
    if (!page)
    {
        page = pmem_alloc_page();
        tmp = vmem_physical(page);
        memset(tmp, 0, PAGESIZE);
        table[index] = page | 1;
    }
    return vmem_physical(page);
}

void vmem_map(uint64_t cr3, void* vaddr, page_addr page, int flags)
{
    (void)flags;

    uint16_t a;
    uint16_t b;
    uint16_t c;
    uint16_t d;

    page_addr* table;

    a = (((uint64_t)vaddr) >> (3 + 9 * 4)) & 0x1ff;
    b = (((uint64_t)vaddr) >> (3 + 9 * 3)) & 0x1ff;
    c = (((uint64_t)vaddr) >> (3 + 9 * 2)) & 0x1ff;
    d = (((uint64_t)vaddr) >> (3 + 9 * 1)) & 0x1ff;

    table = vmem_physical(cr3);
    table = _ensure_mapped(table, a);
    table = _ensure_mapped(table, b);
    table = _ensure_mapped(table, c);
    table[d] = page | 1;
}

void vmem_kmap(void* vaddr, page_addr page, int flags)
{
    vmem_map(gKernel.cr3, vaddr, page, flags);
}

void* vmem_io_map(page_addr base, size_t size)
{
    void* addr;

    size = page_size_round(size);
    addr = kheap_alloc(size);
    for (size_t i = 0; i < size / PAGESIZE; ++i)
        vmem_kmap((char*)addr + PAGESIZE * i, base + PAGESIZE * i, 0);
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
    /*
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
        pageA = vmem_recurse(PAGE_RECURSE, PAGE_RECURSE, PAGE_RECURSE, a);
        if (*pageA)
        {
            for (b = 0; b < 512; ++b)
            {
                pageB = vmem_recurse(PAGE_RECURSE, PAGE_RECURSE, a, b);
                if (*pageB)
                {
                    for (c = 0; c < 512; ++c)
                    {
                        pageC = vmem_recurse(PAGE_RECURSE, a, b, c);
                        if (*pageC)
                        {
                            for (d = 0; d < 512; ++d)
                            {
                                pageD = vmem_recurse(a, b, c, d);
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
    }*/

    /* Force a TLB flush */
    /*
    __asm__ __volatile__ (
        "mov %%cr3, %%rax\r\n"
        "mov %%rax, %%cr3\r\n"
        ::: "rax"
    );
    */
}
