#ifndef _SYS__FRAGMENTS_H
#define _SYS__FRAGMENTS_H 1

#include <stdint.h>
#include <stddef.h>

#define PACKED     __attribute__ ((packed))
#define ALIGN(x)   __attribute__ ((aligned (x)))
#define SECTION(x) __attribute__ ((section (x)))

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
