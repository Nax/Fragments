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

#ifndef _KERNEL_ARCH_X86_64_CPU_FEATURES_H
#define _KERNEL_ARCH_X86_64_CPU_FEATURES_H 1

#include <stdint.h>

typedef struct {
    union {
        struct {
            uint32_t cpuid1_edx;
            uint32_t cpuid1_ecx;
            uint32_t cpuid7_ebx;
            uint32_t cpuid7_ecx;
            uint32_t cpuidex1_edx;
            uint32_t cpuidex1_ecx;
        };
        struct {
            unsigned fpu:1;
            unsigned vme:1;
            unsigned de:1;
            unsigned pse:1;
            unsigned tsc:1;
            unsigned msr:1;
            unsigned pae:1;
            unsigned mce:1;
            unsigned cx8:1;
            unsigned apic:1;
            unsigned reserved_0:1;
            unsigned sep:1;
            unsigned mtrr:1;
            unsigned pge:1;
            unsigned mca:1;
            unsigned cmov:1;
            unsigned pat:1;
            unsigned pse_36:1;
            unsigned psn:1;
            unsigned clfsh:1;
            unsigned reserved_1:1;
            unsigned ds:1;
            unsigned acpi:1;
            unsigned mmx:1;
            unsigned fxsr:1;
            unsigned sse:1;
            unsigned sse2:1;
            unsigned ss:1;
            unsigned htt:1;
            unsigned tm:1;
            unsigned ia64:1;
            unsigned pbe:1;
            unsigned sse3:1;
            unsigned pclmulqdq:1;
            unsigned dtes64:1;
            unsigned monitor:1;
            unsigned ds_cpl:1;
            unsigned vmx:1;
            unsigned smx:1;
            unsigned est:1;
            unsigned tm2:1;
            unsigned ssse3:1;
            unsigned cnxt_id:1;
            unsigned sdbg:1;
            unsigned fma:1;
            unsigned cx16:1;
            unsigned xtpr:1;
            unsigned pdcm:1;
            unsigned reserved_2:1;
            unsigned pcid:1;
            unsigned dca:1;
            unsigned sse4_1:1;
            unsigned sse4_2:1;
            unsigned x2apic:1;
            unsigned movbe:1;
            unsigned popcnt:1;
            unsigned tsc_deadline:1;
            unsigned aes:1;
            unsigned xsave:1;
            unsigned osxsave:1;
            unsigned avx:1;
            unsigned f16c:1;
            unsigned rdrnd:1;
            unsigned hypervisor:1;
        };
    };
} CpuFeatures;

void detect_cpu_features(void);
void print_cpu_features(void);

#endif
