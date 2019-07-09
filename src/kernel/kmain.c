#include <string.h>
#include "kernel.h"

KernelContext gKernel;

void kmain(FragmentsKernelInfo* info)
{
    void* addr;

    memcpy(&gKernel.bootInfo, info, sizeof(gKernel.bootInfo));

    gdt_init();
    clear_screen();
    println("Hello world from the kernel!");
    printhex8(gKernel.bootInfo.drive);
    putchar('\n');

    printhex64((uint64_t)kmalloc(0, 16)); putchar('\n');
    printhex64((uint64_t)kmalloc(0, 16)); putchar('\n');
    printhex64((uint64_t)kmalloc(0, 16)); putchar('\n');
    printhex64((uint64_t)kmalloc(0, 16)); putchar('\n');
    printhex64((uint64_t)kmalloc(0, 16)); putchar('\n');

    for (int i = 0; i < 10; ++i)
    {
        addr = kmalloc(0, 32);
        printhex64((uint64_t)addr);
        putchar('\n');
        kfree(addr);
    }

    for (;;) {}
}
