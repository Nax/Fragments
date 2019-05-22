#include "stage2.h"

#define SCREEN_X 80
#define SCREEN_Y 25

static int x = 0;
static int y = 0;
static uint16_t* const screen = (uint16_t*)0xb8000;

void putchar(int c)
{
    uint16_t value;

    value = 0x0F00 | (c & 0xff);
    screen[x + SCREEN_X * y] = value;
    x++;
    if (x >= SCREEN_X)
    {
        x = 0;
        y++;
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
