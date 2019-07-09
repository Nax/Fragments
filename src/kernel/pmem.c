#include "kernel.h"

static page_addr _pmem_alloc_page_watermark(void)
{
    page_addr addr;

    addr = gKernel.bootInfo.pageUseBase + gKernel.bootInfo.pageUseSize;
    gKernel.bootInfo.pageUseSize += PAGESIZE;

    return addr;
}

void pmem_init(void)
{

}

page_addr pmem_alloc_page(void)
{
    PageAllocator* allocator;

    allocator = &gKernel.pageAllocator;
    if (allocator->stack)
    {
        return -1;
    }
    else
        return _pmem_alloc_page_watermark();
}
