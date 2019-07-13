#include <string.h>
#include "kernel.h"

#define SCREEN_X 80
#define SCREEN_Y 25

static int x = 0;
static int y = 0;

void clear_screen(void)
{
    memset(gKernel.screen, 0, SCREEN_X * SCREEN_Y * 2);
    x = 0;
    y = 0;
}

static void scroll(void)
{
    memmove(gKernel.screen, gKernel.screen + SCREEN_X, SCREEN_X * (SCREEN_Y - 1) * 2);
    memset(gKernel.screen + SCREEN_X * (SCREEN_Y - 1), 0, SCREEN_X * 2);
}

void putchar(int c)
{
    uint16_t value;

    if (c == '\n')
    {
        x = SCREEN_X;
    }
    else
    {
        value = 0x0F00 | (c & 0xff);
        gKernel.screen[x + SCREEN_X * y] = value;
        x++;
    }
    if (x >= SCREEN_X)
    {
        x = 0;
        y++;
    }
    if (y >= SCREEN_Y)
    {
        scroll();
        y--;
    }
}

void print(const char* str)
{
    size_t i;

    i = 0;
    while (str[i])
    {
        putchar(str[i]);
        i++;
    }
}

void println(const char* str)
{
    print(str);
    putchar('\n');
}

static void _printhex(uint64_t value, int shift)
{
    static const char* const charset = "0123456789abcdef";
    int nibble;

    print("0x");
    for (;;)
    {
        nibble = (value >> shift) & 0xf;
        putchar(charset[nibble]);
        if (!shift)
            break;
        shift -= 4;
    }
}

void printhex8(uint8_t value)
{
    _printhex(value, 4);
}

void printhex16(uint16_t value)
{
    _printhex(value, 12);
}

void printhex32(uint32_t value)
{
    _printhex(value, 28);
}

void printhex64(uint64_t value)
{
    _printhex(value, 60);
}
