#include <fs/mfs.h>
#include "stage2.h"

static void map_addr(FragmentsKernelInfo* info, void* pagetable, uint64_t flags, uint64_t virtual, void* physical)
{
    void* tmp;
    uint64_t* dir;
    uint16_t index;

    dir = (uint64_t*)pagetable;
    for (int i = 0; i < 3; ++i)
    {
        index = (virtual >> (39 - (9 * i))) & 0x1ff;
        if (dir[index] == 0)
        {
            tmp = alloc_page(info);
            memset(tmp, 0, 4096);
            dir[index] = ((uint32_t)tmp | 1);
        }
        dir = ((uint64_t*)(dir[index] & ~(0xfff)));
    }
    index = (virtual >> 12) & 0x1ff;
    dir[index] = (((uint64_t)physical) | 1 | flags);
}

void load_kernel(FragmentsKernelInfo* info, const MfsPartition* part, uint32_t inode)
{
    uint64_t    kernelBase;
    uint64_t    kernelEntry;
    char        tmp[4096];
    uint64_t    data[15];
    uint64_t*   pagetable;
    void*       page;

    pagetable = alloc_page(info);
    memset(pagetable, 0, 4096);

    /* Identity map the first 16 megabyte */
    for (int i = 0; i < 256 * 16; ++i)
    {
        map_addr(info, pagetable, 0x02, i * 0x1000, (void*)(i * 0x1000));
    }

    /* Recursively map the top level page table */
    pagetable[510] = (uint64_t)pagetable | 0x03;

    /* Load the kernel */
    /* Note: elf parser is rather crude */
    mfs_read(tmp, part, inode);
    for (int i = 0; i < 15; ++i)
    {
        data[i] = ((MfsFileChunk*)tmp)->data[i + 1];
    }
    kernelBase = 0xffffffff80000000;
    mfs_read(tmp, part, ((MfsFileChunk*)tmp)->data[0]);
    memcpy(&kernelEntry, tmp + 0x18, 8);
    for (int i = 0; i < 15; ++i)
    {
        inode = data[i];
        if (!inode)
            break;
        page = alloc_page(info);
        mfs_read(page, part, inode);
        map_addr(info, pagetable, 2, kernelBase, page);
        kernelBase += 0x1000;
    }

    enter_kernel(info, pagetable, kernelEntry & 0xffffffff, kernelEntry >> 32);
}
