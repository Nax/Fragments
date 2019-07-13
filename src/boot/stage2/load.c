#include <fs/mfs.h>
#include <elf.h>
#include "stage2.h"

static void _map_addr(FragmentsKernelInfo* info, void* pagetable, uint64_t flags, uint64_t virtual, void* physical)
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
            tmp = alloc_pages(info, PAGESIZE);
            memset(tmp, 0, PAGESIZE);
            dir[index] = ((uint32_t)tmp | 1);
        }
        dir = ((uint64_t*)(uintptr_t)(dir[index] & ~(0xfff)));
    }
    index = (virtual >> 12) & 0x1ff;
    dir[index] = (((uintptr_t)physical) | 1 | flags);
}

static void _map_addr_range(FragmentsKernelInfo* info, void* pagetable, uint64_t flags, uint64_t virtual, void* physical, size_t size)
{
    size_t count;

    count = ((size - 1) / PAGESIZE) + 1;

    for (size_t i = 0; i < count; ++i)
        _map_addr(info, pagetable, flags, virtual + i * PAGESIZE, (char*)physical + i * PAGESIZE);
}

static void load_image(FragmentsKernelInfo* info, uint64_t* pagetable, const MfsPartition* part, uint32_t inode)
{
    char tmp[MFS_CHUNK];
    MfsFileChunk* file;
    char* kimage;

    (void)pagetable;
    mfs_read(tmp, part, inode);
    file = (MfsFileChunk*)tmp;
    info->kimageSize = file->size;
    kimage = alloc_pages(info, info->kimageSize);
    mfs_read_file(kimage, part, inode, 0, info->kimageSize);
    info->kimage = (uintptr_t)kimage;
}

static void load_kernel(FragmentsKernelInfo* info, uint64_t* pagetable, const MfsPartition* part, uint32_t inode)
{
    Elf64_Ehdr  ehdr;
    Elf64_Phdr  phdr;
    void*       tmp;

    /* Load the kernel */
    /* Note: elf parser is rather crude */
    mfs_read_file(&ehdr, part, inode, 0, sizeof(ehdr));
    for (int i = 0; i < ehdr.e_phnum; ++i)
    {
        mfs_read_file(&phdr, part, inode, ehdr.e_phoff + ehdr.e_phentsize * i, sizeof(phdr));
        tmp = alloc_pages(info, phdr.p_memsz);
        memset(tmp, 0, phdr.p_memsz);
        mfs_read_file((char*)tmp, part, inode, phdr.p_offset, phdr.p_filesz);
        _map_addr_range(info, pagetable, 0, phdr.p_vaddr, tmp, phdr.p_memsz);
    }

    enter_kernel(info, pagetable, ehdr.e_entry & 0xffffffff, ehdr.e_entry >> 32);
}

void load(FragmentsKernelInfo* info, const MfsPartition* part, uint32_t inodeKernel, uint32_t inodeImage)
{
    uint64_t*   pagetable;

    pagetable = alloc_pages(info, PAGESIZE);
    memset(pagetable, 0, PAGESIZE);

    /* Identity map the first 16 megabyte */
    for (int i = 0; i < 256 * 16; ++i)
    {
        _map_addr(info, pagetable, 0x02, i * 0x1000, (void*)(i * 0x1000));
    }

    /* Recursively map the top level page table */
    pagetable[510] = (uintptr_t)pagetable | 0x03;

    load_image(info, pagetable, part, inodeImage);
    load_kernel(info, pagetable, part, inodeKernel);
}
