#include <string.h>
#include "kernel.h"

void addr_space_create(AddressSpace* addrSpace)
{
    page_addr pageTablePhysical;
    page_addr* pageTable;

    pageTablePhysical = pmem_alloc_page();
    pageTable = gKernel.kernelThreads[0].tmpPageAddr;
    vmem_map(pageTable, pageTablePhysical, 0);
    memset(pageTable, 0, PAGESIZE);
    pageTable[PAGE_KERNEL] = *vmem_recurse(PAGE_RECURSE, PAGE_RECURSE, PAGE_RECURSE, PAGE_KERNEL) | 1;
    pageTable[PAGE_RECURSE] = *vmem_recurse(PAGE_RECURSE, PAGE_RECURSE, PAGE_RECURSE, PAGE_RECURSE) | 1;
    addrSpace->cr3 = pageTablePhysical;
}

void addr_space_switch(AddressSpace* addrSpace)
{
    __asm__ __volatile__ ("mov %%rax, %%cr3\r\n" :: "a"(addrSpace->cr3));
}
