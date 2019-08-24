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
