#ifndef _SYS_IO_H
#define _SYS_IO_H 1

#include <stdint.h>

static inline uint8_t in8(uint16_t port)
{
    uint8_t value;

    __asm__ __volatile__ ("in %1, %0\r\n" : "=a"(value) : "Nd"(port));
    return value;
}

static inline uint8_t in16(uint16_t port)
{
    uint16_t value;

    __asm__ __volatile__ ("in %1, %0\r\n" : "=a"(value) : "Nd"(port));
    return value;
}

static inline uint8_t in32(uint16_t port)
{
    uint32_t value;

    __asm__ __volatile__ ("in %1, %0\r\n" : "=a"(value) : "Nd"(port));
    return value;
}

static inline void out8(uint16_t port, uint8_t value)
{
    __asm__ __volatile__ ("out %0, %1\r\n" :: "a"(value), "Nd"(port));
}

static inline void out16(uint16_t port, uint16_t value)
{
    __asm__ __volatile__ ("out %0, %1\r\n" :: "a"(value), "Nd"(port));
}

static inline void out32(uint16_t port, uint32_t value)
{
    __asm__ __volatile__ ("out %0, %1\r\n" :: "a"(value), "Nd"(port));
}

static inline void io_wait(void)
{
    out8(0x80, 0);
}

#endif
