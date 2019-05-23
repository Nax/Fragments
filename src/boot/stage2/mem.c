#include "stage2.h"

void detect_memory(void)
{
    int carry;
    Regs r;
    MemRegion mem;

    println("===== MEMORY MAP =====");
    r.ebx = 0;
    for (;;)
    {
        mem.extended = 1;
        r.eax = 0xe820;
        r.ecx = 24;
        r.edx = 0x534d4150;
        r.edi = (uint32_t)&mem;
        carry = bios(0x15, &r);
        if (carry || r.ebx == 0)
            break;
        if (mem.size == 0)
            continue;
        print("Mem Region: ");
        printhex64(mem.base);
        putchar(' ');
        printhex64(mem.size);
        putchar(' ');
        printhex32(mem.type);
        putchar('\n');
    }
    println("======================\n");
}
