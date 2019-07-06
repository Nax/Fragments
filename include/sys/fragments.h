#ifndef _SYS__FRAGMENTS_H
#define _SYS__FRAGMENTS_H 1

#include <stdint.h>
#include <stddef.h>

#if defined(__GNUC__)
# define PACKED     __attribute__ ((packed))
# define ALIGN(x)   __attribute__ ((aligned (x)))
# define SECTION(x) __attribute__ ((section (x)))
#else
# define PACKED
# define ALIGN(x)
# define SECTION(x)
#endif

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
    FragmentsMbrEntry   mbrEntry;
    size_t              memRegionCount;
    FragmentsMemRegion  memRegions[64];
    uint64_t            pageUseBase;
    uint64_t            pageUseSize;
} FragmentsKernelInfo;

#endif
