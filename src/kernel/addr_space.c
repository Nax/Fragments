#include <string.h>
#include "kernel.h"

void addr_space_create(AddressSpace* addrSpace)
{
    page_addr cr3;
    page_addr* table;
    page_addr* ktable;

    cr3 = pmem_alloc_page();
    table = vmem_physical(cr3);
    ktable = vmem_physical(gKernel.cr3);

    memset(table, 0, PAGESIZE);
    for (size_t i = 0x1f0; i < 0x200; ++i)
        table[i] = ktable[i];
    addrSpace->cr3 = cr3;
}

void addr_space_switch(AddressSpace* addrSpace)
{
    __asm__ __volatile__ ("mov %%rax, %%cr3\r\n" :: "a"(addrSpace->cr3));
}
