#include <string.h>
#include <elf.h>
#include "kernel.h"

static ProcessData gProc;

pid_t process_create(void)
{
    memset(&gProc, 0, sizeof(gProc));
    addr_space_create(&gProc.addrSpace);
    return 1;
}

static void _allocProcSection(uint64_t addr, const char* fileData, size_t fileSize, size_t memSize)
{
    size_t pageCount;
    page_addr paddr;
    char* vaddr;
    size_t readSize;

    pageCount = page_size_round(memSize);
    for (size_t i = 0; i < pageCount; ++i)
    {
        paddr = pmem_alloc_page();
        vaddr = (char*)(addr + i * PAGESIZE);
        vmem_map(vaddr, paddr, 0);
        if (i * PAGESIZE >= fileSize)
            readSize = 0;
        else
        {
            readSize = fileSize - i * PAGESIZE;
            if (readSize > PAGESIZE)
                readSize = PAGESIZE;
        }
        memcpy(vaddr, fileData + i * PAGESIZE, readSize);
        memset(vaddr + readSize, 0, PAGESIZE - readSize);
    }
}

void process_elf_load(pid_t process, const char* data)
{
    (void)process;

    Elf64_Ehdr ehdr;
    Elf64_Phdr phdr;

    addr_space_switch(&gProc.addrSpace);
    memcpy(&ehdr, data, sizeof(ehdr));
    for (int i = 0; i < ehdr.e_phnum; ++i)
    {
        memcpy(&phdr, data + ehdr.e_phoff + i * ehdr.e_phentsize, sizeof(phdr));
        _allocProcSection(phdr.p_vaddr, data + phdr.p_offset, phdr.p_filesz, phdr.p_memsz);
    }

    /* Allocate the stack */
    _allocProcSection(0x10000000, NULL, 0, PAGESIZE * 2);
    gProc.state.rsp = 0x10000000 + PAGESIZE * 2;
    gProc.state.rip = ehdr.e_entry;
}

void process_run(pid_t process)
{

}
