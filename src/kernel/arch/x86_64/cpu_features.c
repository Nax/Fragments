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
#include "../../kernel.h"

static inline void cpuid2(uint32_t* dst, uint32_t fn, uint32_t subFn)
{
    __asm__ __volatile__ (
        "cpuid\r\n"
        : "=a" (dst[0]), "=b" (dst[1]), "=c" (dst[2]), "=d" (dst[3])
        : "a" (fn), "c"(subFn)
    );
}

static inline void cpuid(uint32_t* dst, uint32_t fn)
{
    cpuid2(dst, fn, 0);
}

void detect_cpu_features(void)
{
    CpuFeatures* features;
    uint32_t info[4];
    uint32_t maxFn;
    uint32_t maxFnExt;

    features = &gKernel.cpuFeatures;

    memset(features, sizeof(*features), 0);

    cpuid(info, 0);
    maxFn = info[0];

    cpuid(info, 0x80000000);
    maxFnExt = info[0];

    if (maxFn >= 1)
    {
        cpuid(info, 1);
        features->cpuid1_edx = info[3];
        features->cpuid1_ecx = info[2];
    }

    if (maxFn >= 7)
    {
        cpuid2(info, 7, 0);
        features->cpuid7_ebx = info[1];
        features->cpuid7_ecx = info[2];
    }

    if (maxFnExt >= 0x80000001)
    {
        cpuid(info, 0x80000001);
        features->cpuidex1_edx = info[3];
        features->cpuidex1_ecx = info[2];
    }
}

#define STR(x)              #x
#define XSTR(x)             STR(x)
#define PRINT_FEATURE(x)    do { if (features->x) { print(XSTR(x)); putchar(' '); } } while (0)

void print_cpu_features(void)
{
    const CpuFeatures* features;
    features = &gKernel.cpuFeatures;

    PRINT_FEATURE(fpu);
    PRINT_FEATURE(vme);
    PRINT_FEATURE(de);
    PRINT_FEATURE(pse);
    PRINT_FEATURE(tsc);
    PRINT_FEATURE(msr);
    PRINT_FEATURE(pae);
    PRINT_FEATURE(mce);
    PRINT_FEATURE(cx8);
    PRINT_FEATURE(apic);
    PRINT_FEATURE(sep);
    PRINT_FEATURE(mtrr);
    PRINT_FEATURE(pge);
    PRINT_FEATURE(mca);
    PRINT_FEATURE(cmov);
    PRINT_FEATURE(pat);
    PRINT_FEATURE(pse_36);
    PRINT_FEATURE(psn);
    PRINT_FEATURE(clfsh);
    PRINT_FEATURE(ds);
    PRINT_FEATURE(acpi);
    PRINT_FEATURE(mmx);
    PRINT_FEATURE(fxsr);
    PRINT_FEATURE(sse);
    PRINT_FEATURE(sse2);
    PRINT_FEATURE(ss);
    PRINT_FEATURE(htt);
    PRINT_FEATURE(tm);
    PRINT_FEATURE(ia64);
    PRINT_FEATURE(pbe);
    PRINT_FEATURE(sse3);
    PRINT_FEATURE(pclmulqdq);
    PRINT_FEATURE(dtes64);
    PRINT_FEATURE(monitor);
    PRINT_FEATURE(ds_cpl);
    PRINT_FEATURE(vmx);
    PRINT_FEATURE(smx);
    PRINT_FEATURE(est);
    PRINT_FEATURE(tm2);
    PRINT_FEATURE(ssse3);
    PRINT_FEATURE(cnxt_id);
    PRINT_FEATURE(sdbg);
    PRINT_FEATURE(fma);
    PRINT_FEATURE(cx16);
    PRINT_FEATURE(xtpr);
    PRINT_FEATURE(pdcm);
    PRINT_FEATURE(reserved_2);
    PRINT_FEATURE(pcid);
    PRINT_FEATURE(dca);
    PRINT_FEATURE(sse4_1);
    PRINT_FEATURE(sse4_2);
    PRINT_FEATURE(x2apic);
    PRINT_FEATURE(movbe);
    PRINT_FEATURE(popcnt);
    PRINT_FEATURE(tsc_deadline);
    PRINT_FEATURE(aes);
    PRINT_FEATURE(xsave);
    PRINT_FEATURE(osxsave);
    PRINT_FEATURE(avx);
    PRINT_FEATURE(f16c);
    PRINT_FEATURE(rdrnd);
    PRINT_FEATURE(hypervisor);

    putchar('\n');
}
