#ifndef _RDISK_H_
#define _RDISK_H_ 1

#include <stdint.h>

typedef struct {
    uint32_t    fileCount;
    uint32_t    fileSectionOffset;
} RDiskHeader;

typedef struct {
    uint32_t    nameOffset;
    uint32_t    nameSize;
    uint32_t    offset;
    uint32_t    size;
} RDiskFile;

#endif
