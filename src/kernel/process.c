/*
 * Copyright (c) 2019, Maxime Bacoux
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <string.h>
#include <elf.h>
#include "kernel.h"

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
