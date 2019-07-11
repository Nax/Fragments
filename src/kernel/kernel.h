#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <fragments.h>

#define VMEM_USED   0x00000001
#define ASM         __asm__ __volatile__

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

/* io ports */
static inline uint8_t in8(uint16_t port)
{
    uint8_t value;

    ASM("in %1, %0\r\n" : "=a"(value) : "Nd"(port));
    return value;
}

static inline uint8_t in16(uint16_t port)
{
    uint16_t value;

    ASM("in %1, %0\r\n" : "=a"(value) : "Nd"(port));
    return value;
}

static inline uint8_t in32(uint16_t port)
{
    uint32_t value;

    ASM("in %1, %0\r\n" : "=a"(value) : "Nd"(port));
    return value;
}

static inline void out8(uint16_t port, uint8_t value)
{
    ASM("out %0, %1\r\n" :: "a"(value), "Nd"(port));
}

static inline void out16(uint16_t port, uint16_t value)
{
    ASM("out %0, %1\r\n" :: "a"(value), "Nd"(port));
}

static inline void out32(uint16_t port, uint32_t value)
{
    ASM("out %0, %1\r\n" :: "a"(value), "Nd"(port));
}

static inline void io_wait(void)
{
    out8(0x80, 0);
}

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

/* wait */
void kernel_wait(void);

/* irq */
void irq_init(void);
void irq_disable_all(void);
void irq_enable(int line);
void irq_disable(int line);

#endif
