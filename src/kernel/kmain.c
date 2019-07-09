#include <string.h>
#include "kernel.h"

KernelContext gKernel;

void kmain(FragmentsKernelInfo* info)
{
    memcpy(&gKernel.bootInfo, info, sizeof(gKernel.bootInfo));

    clear_screen();
    println("Hello world from the kernel!");
    printhex8(gKernel.bootInfo.drive);
    putchar('\n');

    pmem_init();
    gdt_init();

    for (;;) {}
}
