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

#include <sys/io.h>
#include "kernel.h"

#define PIC1_COMMAND    0x0020
#define PIC1_DATA       0x0021
#define PIC2_COMMAND    0x00a0
#define PIC2_DATA       0x00a1

#define ICW1_ICW4	    0x01
#define ICW1_SINGLE	    0x02
#define ICW1_INTERVAL4	0x04
#define ICW1_LEVEL	    0x08
#define ICW1_INIT	    0x10

#define ICW4_8086	    0x01
#define ICW4_AUTO	    0x02
#define ICW4_BUF_SLAVE	0x08
#define ICW4_BUF_MASTER	0x0C
#define ICW4_SFNM	    0x10

void irq_init(void)
{
    uint8_t mask[2];

    mask[0] = in8(PIC1_DATA);
    mask[1] = in8(PIC2_DATA);

    out8(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    out8(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    out8(PIC1_DATA, 0x20);
    io_wait();
    out8(PIC2_DATA, 0x28);
    io_wait();
    out8(PIC1_DATA, 4);
    io_wait();
    out8(PIC2_DATA, 2);
    io_wait();
    out8(PIC1_DATA, ICW4_8086);
    io_wait();
    out8(PIC2_DATA, ICW4_8086);
    io_wait();

    out8(PIC1_DATA, mask[0]);
    out8(PIC2_DATA, mask[1]);
}

void irq_disable_all(void)
{
    out8(PIC1_DATA, 0xff);
    out8(PIC2_DATA, 0xff);
}

void irq_enable(int line)
{
    uint8_t port;
    uint8_t mask;

    if (line < 8)
        port = PIC1_DATA;
    else
        port = PIC2_DATA;

    mask = in8(port);
    mask &= ~(1 << line);
    out8(port, mask);
}

void irq_disable(int line)
{
    uint8_t port;
    uint8_t mask;

    if (line < 8)
        port = PIC1_DATA;
    else
        port = PIC2_DATA;

    mask = in8(port);
    mask |= (1 << line);
    out8(port, mask);
}
