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
