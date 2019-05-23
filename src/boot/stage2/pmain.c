#include "stage2.h"

void pmain(void)
{
    println("Bootloader loaded!\n");
    detect_memory();

    for (;;) {}
}
