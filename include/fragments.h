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

#ifndef _SYS__FRAGMENTS_H
#define _SYS__FRAGMENTS_H 1

#include <stdint.h>
#include <stddef.h>
#include <sys/compiler.h>

#define PAGESIZE    4096

typedef uint64_t page_addr;

typedef struct {
    uint64_t base;
    uint64_t size;
    uint32_t type;
    uint32_t extended;
} PACKED FragmentsMemRegion;

typedef struct {
    uint8_t     status;
    uint8_t     chsFirst[3];
    uint8_t     type;
    uint8_t     chsLast[3];
    uint32_t    lbaFirst;
    uint32_t    lbaSize;
} PACKED FragmentsMbrEntry;

typedef struct {
    uint8_t             drive;
    uint8_t             zero[7];
    FragmentsMbrEntry   mbrEntry;
    uint32_t            memRegionCount;
    uint32_t            padding;
    FragmentsMemRegion  memRegions[64];
    uint64_t            pageUseBase;
    uint64_t            pageUseSize;
    uint64_t            kimage;
    uint64_t            kimageSize;
} PACKED FragmentsKernelInfo;

#endif
