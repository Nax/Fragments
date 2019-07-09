#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

/* print */
void clear_screen(void);
void putchar(int c);
void print(const char* str);
void println(const char* str);
void printhex8(uint8_t value);
void printhex16(uint16_t value);
void printhex32(uint32_t value);
void printhex64(uint64_t value);

#endif
