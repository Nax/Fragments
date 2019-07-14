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

#include <string.h>
#include <rdisk.h>
#include "kernel.h"

void kimage_init(void)
{
    gKernel.kimageSize = gKernel.bootInfo.kimageSize;
    gKernel.kimage = kmalloc(0, gKernel.kimageSize);
    printhex64((uint64_t)gKernel.kimageSize); putchar('\n');
    printhex64((uint64_t)gKernel.kimage); putchar('\n');
    memcpy(gKernel.kimage, (void*)gKernel.bootInfo.kimage, gKernel.kimageSize);
}

const char* kimage_open(const char* path)
{
    size_t pathLen;
    RDiskHeader header;
    RDiskFile file;
    const char* kimageBase;

    kimageBase = gKernel.kimage;
    memcpy(&header, kimageBase, sizeof(header));

    pathLen = strlen(path);
    for (uint32_t i = 0; i < header.fileCount; ++i)
    {
        memcpy(&file, kimageBase + header.fileSectionOffset + i * sizeof(file), sizeof(file));
        if (file.nameSize == pathLen && strncmp(path, kimageBase + file.nameOffset, pathLen) == 0)
        {
            return kimageBase + file.offset;
        }
    }
    return NULL;
}
