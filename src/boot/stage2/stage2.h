#ifndef STAGE2_H
#define STAGE2_H

#if defined(__GNUC__)
# define PACKED __attribute__ ((packed))
#else
# define PACKED
#endif

#include <stdint.h>
#include <stddef.h>

/* Mini libc */
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* dest, int value, size_t n);

typedef struct {
    uint8_t     size;
    uint8_t     zero;
    uint16_t    count;

} PACKED DiskTransferPacket;

typedef struct {
    uint16_t ax;
    uint16_t bx;
    uint16_t cx;
    uint16_t dx;
    uint16_t si;
    uint16_t di;
} PACKED Regs;

int bios(int num, Regs* regs);

#endif
