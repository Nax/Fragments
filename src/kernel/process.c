#include <string.h>
#include <elf.h>
#include "kernel.h"

#define RFLAGS_CF   (1 << 0)
#define RFLAGS_PF   (1 << 2)
#define RFLAGS_AF   (1 << 4)
#define RFLAGS_ZF   (1 << 6)
#define RFLAGS_SF   (1 << 7)
#define RFLAGS_TF   (1 << 8)
#define RFLAGS_IF   (1 << 9)
#define RFLAGS_DF   (1 << 10)
#define RFLAGS_OF   (1 << 11)
#define RFLAGS_NT   (1 << 14)
#define RFLAGS_RF   (1 << 16)
#define RFLAGS_VM   (1 << 17)
#define RFLAGS_AC   (1 << 18)
#define RFLAGS_VIF  (1 << 19)
#define RFLAGS_VIP  (1 << 20)
#define RFLAGS_ID   (1 << 21)

static ProcessData gProc;

static uint64_t _rflags(void)
{
    uint64_t flags;

    __asm__ __volatile__ (
        "pushfq\r\n"
        "popq %0\r\n"
        : "=r"(flags)
    );

    return flags;
}

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
    for (size_t i = 0; i < pageCount; ++i)
    {
        paddr = pmem_alloc_page();
        vaddr = (char*)(addr + i * PAGESIZE);
        kaddr = vmem_physical(paddr);
        vmem_map(cr3, vaddr, paddr, (PAGE_USER | PAGE_READ | PAGE_WRITE | PAGE_EXECUTE));

        if (i * PAGESIZE >= fileSize)
            readSize = 0;
        else
        {
            readSize = fileSize - i * PAGESIZE;
            if (readSize > PAGESIZE)
                readSize = PAGESIZE;
        }

        memcpy(kaddr, fileData + i * PAGESIZE, readSize);
        memset(kaddr + readSize, 0, PAGESIZE - readSize);
    }
}

void process_elf_load(pid_t process, const char* data)
{
    (void)process;

    uint64_t flags;
    Elf64_Ehdr ehdr;
    Elf64_Phdr phdr;

    memcpy(&ehdr, data, sizeof(ehdr));
    printhex64(ehdr.e_entry);
    for (int i = 0; i < ehdr.e_phnum; ++i)
    {
        memcpy(&phdr, data + ehdr.e_phoff + i * ehdr.e_phentsize, sizeof(phdr));
        _allocProcSection(phdr.p_vaddr, data + phdr.p_offset, phdr.p_filesz, phdr.p_memsz);
    }

    /* Allocate the stack */
    _allocProcSection(0x10000000, NULL, 0, PAGESIZE * 2);
    gProc.state.rsp = 0x10000000 + PAGESIZE * 2;
    gProc.state.rip = ehdr.e_entry;

    flags = _rflags();
    flags &= ~(RFLAGS_CF | RFLAGS_PF | RFLAGS_AF | RFLAGS_ZF | RFLAGS_SF
        | RFLAGS_TF | RFLAGS_DF | RFLAGS_OF
        | RFLAGS_NT | RFLAGS_RF | RFLAGS_VM | RFLAGS_AC);
    flags |= RFLAGS_IF;
    gProc.state.rflags = flags;
}

void process_run(pid_t process)
{
    addr_space_switch(&gProc.addrSpace);
    process_return(&gProc.state);
}
