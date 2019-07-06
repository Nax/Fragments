#ifndef STAGE2_H
#define STAGE2_H

#if defined(__GNUC__)
# define PACKED     __attribute__ ((packed))
# define ALIGN(x)   __attribute__ ((aligned (x)))
# define SECTION(x) __attribute__ ((section (x)))
#else
# define PACKED
# define ALIGN(x)
# define SECTION(x)
#endif

#define SECTOR_SIZE         512
#define CHUNK_SIZE          4096
#define SECTORS_PER_CHUNK   (CHUNK_SIZE / SECTOR_SIZE)

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t     size;
    uint8_t     zero;
    uint16_t    count;
    uint16_t    dstOffset;
    uint16_t    dstSegment;
    uint32_t    srcLo;
    uint32_t    srcHi;
} PACKED ALIGN(4) DiskTransferPacket;

typedef struct {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
} PACKED Regs;

typedef struct {
    uint64_t base;
    uint64_t size;
    uint32_t type;
    uint32_t extended;
} PACKED MemRegion;

typedef struct {
    uint8_t     status;
    uint8_t     chsFirst[3];
    uint8_t     type;
    uint8_t     chsLast[3];
    uint32_t    lbaFirst;
    uint32_t    lbaLast;
} PACKED MbrEntry;

typedef struct {
    uint8_t     drive;
    uint32_t    inodeRoot;
    uint32_t    lbaPartition;
    uint32_t    lbaSuperchunk;
    uint32_t    lbaStart;
    uint32_t    lbaRoot;
} MfsPartition;

/* minic.c */
void*   memcpy(void* dest, const void* src, size_t n);
void*   memmove(void* dest, const void* src, size_t n);
void*   memset(void* dest, int value, size_t n);
int     strncmp(const char* a, const char* b, size_t n);

/* print.c */
void putchar(int c);
void print(const char* str);
void println(const char* str);
void printhex8(uint8_t value);
void printhex16(uint16_t value);
void printhex32(uint32_t value);
void printhex64(uint64_t value);

/* mem */
void detect_memory(void);

/* disk.c */
void disk_read_raw(void* dst, uint8_t drive, uint32_t src, uint16_t count);

/* mfs */
void        mfs_init(MfsPartition* part, uint8_t drive, const MbrEntry* mbrEntry);
uint32_t    mfs_seek_child(const MfsPartition* part, uint32_t parent, const char* name);

/* bios.asm */
int bios(int num, Regs* regs);

#endif
