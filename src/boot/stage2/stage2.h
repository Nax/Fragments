#ifndef STAGE2_H
#define STAGE2_H

#if defined(__GNUC__)
# define PACKED __attribute__ ((packed))
#else
# define PACKED
#endif

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t     size;
    uint8_t     zero;
    uint16_t    count;

} PACKED DiskTransferPacket;

typedef struct {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
} PACKED Regs;

typedef struct {
    uint64_t base;
    uint64_t size;
    uint32_t type;
    uint32_t extended;
} PACKED MemRegion;

/* minic.c */
void* memcpy(void* dest, const void* src, size_t n);
void* memmove(void* dest, const void* src, size_t n);
void* memset(void* dest, int value, size_t n);

/* print.c */
void putchar(int c);
void print(const char* str);
void println(const char* str);
void printhex8(uint8_t value);
void printhex16(uint16_t value);
void printhex32(uint32_t value);
void printhex64(uint64_t value);

/* mem */
void detect_memory(void);

/* --- */
int bios(int num, Regs* regs);

#endif
