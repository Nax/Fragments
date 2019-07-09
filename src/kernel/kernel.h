#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <fragments.h>

#define VMEM_USED   0x00000001

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

typedef struct
{
    FragmentsKernelInfo     bootInfo;
    PageAllocator           pageAllocator;
    KernelMemoryAllocator   memoryAllocator;
    size_t                  heapSize;
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
void gdt_load(const void* gdtEntries, size_t count);

#endif
