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

#ifndef MFS_H
#define MFS_H

#define MFS_CHUNK   4096

#include <stdint.h>

#ifndef PACKED
# if defined(__GNUC__)
#  define PACKED __attribute__ ((packed))
# else
#  define PACKED
# endif
#endif

typedef struct
{
    uint64_t    root;
    uint8_t     unused[4088];
} PACKED MfsSuperChunk;

typedef struct
{
    uint16_t    uid;
    uint16_t    gid;
    uint32_t    mode;
    uint64_t    btime;
    uint64_t    atime;
    uint64_t    mtime;
    uint64_t    ctime;
    uint64_t    size;
    uint64_t    data[16];
    uint64_t    idata;
    uint64_t    idata2;
    uint64_t    idata3;
    uint64_t    idata4;
    char        unused[0xf40];
} PACKED MfsFileChunk;

#endif
