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
