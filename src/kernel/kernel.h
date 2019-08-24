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

#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <fragments.h>
#include <sys/types.h>

#define BREAKPOINT do { __asm__ __volatile__ ("xchg %bx, %bx\r\n"); } while (0)

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

#define VMEM_USED   0x00000001
#define ASM         __asm__ __volatile__

#define GDT_CODE_RING0      0x08
#define GDT_DATA_RING0      (GDT_CODE_RING0 + 0x08)
#define GDT_CODE_RING3_32   0x18
#define GDT_DATA_RING3      (GDT_CODE_RING3_32 + 0x08)
#define GDT_CODE_RING3      (GDT_CODE_RING3_32 + 0x10)
#define GDT_TSS_BASE    0x30

#define DESC_TYPE_TSS        0x9
#define DESC_TYPE_INTERRUPT  0xe

#define PROCESS_STATUS_NORMAL   0x00
#define PROCESS_STATUS_SYSCALL  0x01

#define PAGE_KERNEL     0x1ff
#define PAGE_BASE_ENTRY 0x1f0
#define PAGE_BASE       (0xffff000000000000 | (((uint64_t)PAGE_BASE_ENTRY) << 39))

#define PAGE_READ       (1 << 0)
#define PAGE_WRITE      (1 << 1)
#define PAGE_EXECUTE    (1 << 2)
#define PAGE_USER       (1 << 3)

#include <kernel/arch/x86_64/cpu_features.h>

typedef struct
{
    size_t      count;
    size_t      capacity;
    page_addr*  stack;
} PageAllocator;

typedef struct
{
    uint32_t offset;
    uint32_t size;
} KernelFreeBlock;

typedef struct
{
    uint32_t size;
} KernelBlockHeader;

typedef struct
{
    size_t              freeBlockCount;
    size_t              freeBlockCapacity;
    KernelFreeBlock*    freeBlocks;
} KernelMemoryAllocator;

typedef struct {
    uint32_t    reserved0;
    uint32_t    rsp0Lo;
    uint32_t    rsp0Hi;
    uint32_t    rsp1Lo;
    uint32_t    rsp1Hi;
    uint32_t    rsp2Lo;
    uint32_t    rsp2Hi;
    uint32_t    reserved1;
    uint32_t    reserved2;
    uint32_t    ist1Lo;
    uint32_t    ist1Hi;
    uint32_t    ist2Lo;
    uint32_t    ist2Hi;
    uint32_t    ist3Lo;
    uint32_t    ist3Hi;
    uint32_t    ist4Lo;
    uint32_t    ist4Hi;
    uint32_t    ist5Lo;
    uint32_t    ist5Hi;
    uint32_t    ist6Lo;
    uint32_t    ist6Hi;
    uint32_t    ist7Lo;
    uint32_t    ist7Hi;
    uint32_t    reserved3;
    uint32_t    reserved4;
    uint16_t    reserved5;
    uint16_t    iopb;
} PACKED Tss;

typedef struct {
    uint64_t    cr3;
} PACKED AddressSpace;

typedef struct {
    uint64_t    rax;            /* 0x00 */
    uint64_t    rbx;            /* 0x08 */
    uint64_t    rcx;            /* 0x10 */
    uint64_t    rdx;            /* 0x18 */
    uint64_t    rdi;            /* 0x20 */
    uint64_t    rsi;            /* 0x28 */
    uint64_t    rsp;            /* 0x30 */
    uint64_t    rbp;            /* 0x38 */
    uint64_t    r8;             /* 0x40 */
    uint64_t    r9;             /* 0x48 */
    uint64_t    r10;            /* 0x50 */
    uint64_t    r11;            /* 0x58 */
    uint64_t    r12;            /* 0x60 */
    uint64_t    r13;            /* 0x68 */
    uint64_t    r14;            /* 0x70 */
    uint64_t    r15;            /* 0x78 */
    uint64_t    rip;            /* 0x80 */
    uint64_t    rflags;         /* 0x88 */
    char        fxdata[512];    /* 0x90 */
} PACKED CpuState;

typedef struct {
    CpuState        state;
    AddressSpace    addrSpace;
    uint32_t        nextProcess;
    uint32_t        prevProcess;
    uint32_t        status;
} PACKED ProcessData;

typedef struct _KernelThreadContext
{
    struct _KernelThreadContext*    ctx;
    void*                           stack;
    Tss*                            tss;
    void*                           tmpPageAddr;
} PACKED KernelThreadContext;

typedef struct
{
    uint64_t                cr3;
    uint16_t*               screen;
    char*                   kimage;
    size_t                  kimageSize;
    FragmentsKernelInfo     bootInfo;
    PageAllocator           pageAllocator;
    KernelMemoryAllocator   memoryAllocator;
    size_t                  heapSize;
    KernelThreadContext*    kernelThreads;
    CpuFeatures             cpuFeatures;
} KernelContext;

typedef struct {
    uint64_t rip;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t args[6];
} PACKED SyscallReq;

typedef int64_t (*syscall_ptr)(SyscallReq*);

extern KernelContext gKernel;

inline static size_t page_size_round(size_t size)
{
    if (size == 0)
        return 0;
    return (((size - 1) / PAGESIZE) + 1) * PAGESIZE;
}

/* addr_space */
void addr_space_create(AddressSpace* addrSpace);
void addr_space_switch(AddressSpace* addrSpace);

/* print */
void clear_screen(void);
void putchar(int c);
void print(const char* str);
void println(const char* str);
void printhex8(uint8_t value);
void printhex16(uint16_t value);
void printhex32(uint32_t value);
void printhex64(uint64_t value);

/* kmalloc */
void*   kheap_alloc(size_t size);
void*   kmalloc_raw(size_t size);
void*   kmalloc(int flags, size_t size);
void    kfree(void* addr);

/* pmem */
void        pmem_init(void);
page_addr   pmem_alloc_page(void);
void        pmem_free_page(page_addr page);

/* vmem */
void        vmem_init(void);
void*       vmem_physical(page_addr page);
void        vmem_map(uint64_t cr3, void* vaddr, page_addr page, int flags);
void        vmem_kmap(void* vaddr, page_addr page, int flags);
void*       vmem_io_map(page_addr base, size_t size);
void        vmem_unmap_lower(void);

/* gdt */
void gdt_init(void);
void gdt_load(const void* gdtEntries, size_t count, uint16_t codeSeg, uint16_t dataSeg);
void gdt_describe_tss(uint16_t index, Tss* tss, size_t size);

/* wait */
void kernel_wait(void);

/* irq */
void irq_init(void);
void irq_disable_all(void);
void irq_enable(int line);
void irq_disable(int line);

/* idt */
void idt_init(void);
void idt_register(uint8_t index, uint8_t ist, void* handler);

/* int handlers */
void int_handler_keyboard(void);

/* process */
pid_t   process_create(void);
void    process_elf_load(pid_t process, const char* data);
void    process_run(pid_t process);
void    process_return(CpuState* state);

/* kimage */
void        kimage_init(void);
const char* kimage_open(const char* path);

/* syscall */
void syscall_handler(void);
void syscall_dispatch(unsigned int sysno, SyscallReq* req);
void syscall_return(SyscallReq* req, int64_t value);

#endif
