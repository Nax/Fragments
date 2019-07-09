#include <string.h>
#include "kernel.h"

void kmain(void)
{
    clear_screen();
    println("Hello world from the kernel!");

    for (;;) {}
}
