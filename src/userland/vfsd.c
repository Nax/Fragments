/*
 * VFSd - The Virtual File System daemon
 *
 * (c) 2019 Maxime Bacoux
 *
 * This file is distributed under the terms of the MIT license.
 */

#include <unistd.h>

void main(void)
{
    syscall(0, 0, 0, 0, 0, 0, 0);

    for (;;) {}
}
