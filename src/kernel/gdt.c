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

#include "kernel.h"

typedef struct {
    uint16_t    limitLo;
    uint16_t    baseLo;
    uint8_t     baseMd;
    unsigned    accessed:1;
    unsigned    rw:1;
    unsigned    dc:1;
    unsigned    execute:1;
    unsigned    segment:1;
    unsigned    level:2;
    unsigned    present:1;
    uint8_t     limitHi:4;
    unsigned    zero:1;
    unsigned    longmode:1;
    unsigned    size:1;
    unsigned    granularity:1;
    uint8_t     baseHi;
} PACKED GdtDescriptor;

typedef struct
{
    uint16_t    limit0;
    uint16_t    base0;
    uint8_t     base1;
    unsigned    type:4;
    unsigned    zero0:1;
    unsigned    dpl:2;
    unsigned    present:1;
    unsigned    limit1:4;
    unsigned    avl:1;
    unsigned    zero1:2;
    unsigned    granularity:1;
    uint8_t     base2;
    uint32_t    base3;
    uint32_t    zero2;
} PACKED TssDescriptor;

typedef union
{
    TssDescriptor       tss;
    GdtDescriptor       gdt[2];
} PACKED ALIGN(16) AnyDescriptor;

static AnyDescriptor gDescriptors[32];

static GdtDescriptor* _gdt(uint16_t index)
{
    index /= 8;

    if (!(index & 1))
        return gDescriptors[index / 2].gdt + 0;
    else
        return gDescriptors[index / 2].gdt + 1;
}

static TssDescriptor* _tss(uint16_t index)
{
    index /= 16;

    return &gDescriptors[index].tss;
}

static void _gdtMakeSeg(uint16_t index, int ring, int data)
{
    GdtDescriptor* desc;

    desc = _gdt(index);
    desc->limitLo = 0;
    desc->baseLo = 0;
    desc->baseMd = 0;
    desc->accessed = 0;
    desc->rw = !!(data);
    desc->dc = 0;
    desc->execute = !(data);
    desc->segment = 1;
    desc->level = ring;
    desc->present = 1;
    desc->limitHi = 0;
    desc->zero = 0;
    desc->longmode = !(data);
    desc->size = 0;
    desc->granularity = 0;
    desc->baseHi = 0;
}

void gdt_init(void)
{
    _gdtMakeSeg(GDT_CODE_RING0, 0, 0);
    _gdtMakeSeg(GDT_DATA_RING0, 0, 1);
    _gdtMakeSeg(GDT_CODE_RING3, 3, 0);
    _gdtMakeSeg(GDT_DATA_RING3, 3, 1);
    gdt_load(gDescriptors, sizeof(gDescriptors) - 1, GDT_CODE_RING0, GDT_DATA_RING0);
}

void gdt_describe_tss(uint16_t index, Tss* tss, size_t size)
{
    TssDescriptor* desc;

    desc = _tss(index);
    desc->limit0 = size & 0xffff;
    desc->base0 = (uint64_t)tss & 0xffff;
    desc->base1 = ((uint64_t)tss >> 16) & 0xff;
    desc->type = DESC_TYPE_TSS;
    desc->zero0 = 0;
    desc->dpl = 0;
    desc->present = 1;
    desc->limit1 = (size >> 16) & 0xff;
    desc->avl = 0;
    desc->zero1 = 0;
    desc->granularity = 0;
    desc->base2 = ((uint64_t)tss >> 24) & 0xff;
    desc->base3 = ((uint64_t)tss >> 32) & 0xffffffff;
    desc->zero2 = 0;
}
