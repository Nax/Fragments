#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <fragments.h>

#define VMEM_USED   0x00000001
#define ASM         __asm__ __volatile__

#define GDT_CODE_RING0  0x08
#define GDT_TSS_BASE    0x30

#define DESC_TYPE_TSS        0x9
#define DESC_TYPE_INTERRUPT  0xe

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

typedef struct _KernelThreadContext
{
    struct _KernelThreadContext*    ctx;
    void*                           stack;
    Tss*                            tss;
} KernelThreadContext;

typedef struct
{
    FragmentsKernelInfo     bootInfo;
    PageAllocator           pageAllocator;
    KernelMemoryAllocator   memoryAllocator;
    size_t                  heapSize;
    KernelThreadContext*    kernelThreads;
} KernelContext;

extern KernelContext gKernel;

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
void vmem_map(void* vaddr, page_addr page, int flags);

/* gdt */
void gdt_init(void);
void gdt_load(const void* gdtEntries, size_t count, uint16_t codeSeg);
void gdt_describe_tss(uint16_t index, Tss* tss, size_t size);

/* wait */
void kernel_wait(void);

/* irq */
void irq_init(void);
void irq_disable_all(void);
void irq_enable(int line);
void irq_disable(int line);

#endif
