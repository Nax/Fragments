/*
 * Copyright (c) 2019, Maxime Bacoux
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
