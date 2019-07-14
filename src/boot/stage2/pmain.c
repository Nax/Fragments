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

#include "stage2.h"

void pmain(uint8_t drive, const FragmentsMbrEntry* mbrEntry)
{
    FragmentsKernelInfo kernelInfo;
    MfsPartition part;
    uint32_t inode;
    uint32_t inodeKernel;
    uint32_t inodeImage;

    println("Bootloader loaded!");
    print("Booting from drive ");
    printhex8(drive);
    print(", MBR entry at ");
    printhex32((uint32_t)mbrEntry);
    putchar('\n');
    putchar('\n');

    kernelInfo.drive = drive;
    memcpy(&kernelInfo.mbrEntry, mbrEntry, sizeof(*mbrEntry));

    detect_memory(&kernelInfo);

    mfs_init(&part, drive, mbrEntry);
    inode = part.inodeRoot;

    inode = mfs_seek_child(&part, inode, "boot");
    print("/boot inode:        ");
    printhex32(inode);
    putchar('\n');

    inodeKernel = mfs_seek_child(&part, inode, "kernel");
    print("/boot/kernel inode: ");
    printhex32(inodeKernel);
    putchar('\n');

    inodeImage = mfs_seek_child(&part, inode, "kimage");
    print("/boot/kimage inode: ");
    printhex32(inodeImage);
    putchar('\n');

    load(&kernelInfo, &part, inodeKernel, inodeImage);

    for (;;) {}
}
