#include "stage2.h"

void pmain(void)
{
    Regs r;

    print("Hello, world!");

/*
    memset(&r, 0, sizeof(r));
    r.ax = 0x0900 | 'A';
    r.bx = 0x000F;
    r.cx = 1;
    bios(0x10, &r); */

    for (;;) {}
}
