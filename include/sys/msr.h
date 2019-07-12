#ifndef _SYS_MSR_H
#define _SYS_MSR_H 1

#include <stdint.h>

#define MSR_EFER            0xC0000080
#define MSR_STAR            0xC0000081
#define MSR_LSTAR           0xC0000082
#define MSR_CSTAR           0xC0000083
#define MSR_SFMASK          0xC0000084
#define MSR_FSBASE          0xC0000100
#define MSR_GSBASE          0xC0000101
#define MSR_KERNELGSBASE    0xC0000102

static inline uint64_t msr_read(uint32_t msr)
{
    uint32_t hi;
    uint32_t lo;

    __asm__ __volatile__ ("rdmsr\r\n" : "=a"(lo), "=d"(hi) : "c"(msr));

    return (((uint64_t)hi << 32) | lo);
}

static inline void msr_write(uint32_t msr, uint64_t value)
{
    uint32_t hi;
    uint32_t lo;

    hi = (value >> 32);
    lo = value & 0xffffffff;

    __asm__ __volatile__ ("wrmsr\r\n" :: "a"(lo), "d"(hi), "c"(msr));
}

#endif
