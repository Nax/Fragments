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
    uint64_t cr3;
    size_t pageCount;
    page_addr paddr;
    char* vaddr;
    char* kaddr;
    size_t readSize;

    cr3 = gProc.addrSpace.cr3;
    pageCount = page_size_round(memSize) / PAGESIZE;
    println("6-1");
    for (size_t i = 0; i < pageCount; ++i)
    {
        println("6-2");
        paddr = pmem_alloc_page();
        println("6-3");
        vaddr = (char*)(addr + i * PAGESIZE);
        kaddr = vmem_physical(paddr);
        println("6-4");
        BREAKPOINT;
        vmem_map(cr3, vaddr, paddr, 0);
        println("6-5");

        if (i * PAGESIZE >= fileSize)
            readSize = 0;
        else
        {
            readSize = fileSize - i * PAGESIZE;
            if (readSize > PAGESIZE)
                readSize = PAGESIZE;
        }

        println("6-6");
        BREAKPOINT;
        memcpy(kaddr, fileData + i * PAGESIZE, readSize);
        println("6-7");
        memset(kaddr + readSize, 0, PAGESIZE - readSize);
    }
}

void process_elf_load(pid_t process, const char* data)
{
    (void)process;

    Elf64_Ehdr ehdr;
    Elf64_Phdr phdr;

    println("1");
    println("2");
    memcpy(&ehdr, data, sizeof(ehdr));
    println("3");
    printhex64(ehdr.e_entry);
    println("4");
    for (int i = 0; i < ehdr.e_phnum; ++i)
    {
        println("5");
        memcpy(&phdr, data + ehdr.e_phoff + i * ehdr.e_phentsize, sizeof(phdr));
        println("6");
        _allocProcSection(phdr.p_vaddr, data + phdr.p_offset, phdr.p_filesz, phdr.p_memsz);
        println("7");
    }
    println("8");

    /* Allocate the stack */
    _allocProcSection(0x10000000, NULL, 0, PAGESIZE * 2);
    println("9");
    gProc.state.rsp = 0x10000000 + PAGESIZE * 2;
    gProc.state.rip = ehdr.e_entry;
}

void process_run(pid_t process)
{
    addr_space_switch(&gProc.addrSpace);
    process_return(&gProc.state);
}
