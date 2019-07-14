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
    uint16_t offsetLo;
    uint16_t selector;
    unsigned ist:3;
    unsigned zero0:5;
    unsigned type:4;
    unsigned zero1:1;
    unsigned dpl:2;
    unsigned present:1;
    uint16_t offsetMd;
    uint32_t offsetHi;
    uint32_t zero2;
} PACKED ALIGN(8) IDTDescriptor;

typedef struct {
    uint16_t                size;
    const IDTDescriptor*    descriptors;
} PACKED ALIGN(16) IDT;

IDTDescriptor gIDTDescriptors[0x30];
IDT gIDT;

void idt_init(void)
{
    gIDT.size = sizeof(gIDTDescriptors) - 1;
    gIDT.descriptors = gIDTDescriptors;

    ASM ("lidt %0\r\n" :: "m"(gIDT));
}

void idt_register(uint8_t index, uint8_t ist, void* handler)
{
    uint64_t handlerAddr;

    handlerAddr = (uint64_t)handler;
    gIDTDescriptors[index].offsetLo = (handlerAddr & 0xffff);
    gIDTDescriptors[index].selector = GDT_CODE_RING0;
    gIDTDescriptors[index].ist = ist;
    gIDTDescriptors[index].zero0 = 0;
    gIDTDescriptors[index].type = DESC_TYPE_INTERRUPT;
    gIDTDescriptors[index].zero1 = 0;
    gIDTDescriptors[index].dpl = 0;
    gIDTDescriptors[index].present = 1;
    gIDTDescriptors[index].offsetMd = (handlerAddr >> 16) & 0xffff;
    gIDTDescriptors[index].offsetHi = (handlerAddr >> 32);
    gIDTDescriptors[index].zero2 = 0;
}
