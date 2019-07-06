#include "stage2.h"

SECTION(".data16")
static DiskTransferPacket gDiskTransferPacket;

static void canonize(uint16_t* segment, uint16_t* offset, uint32_t addr)
{
    *segment = (addr >> 4) & 0xf000;
    *offset = addr & 0xffff;
}

void disk_read_raw(void* dst, uint8_t drive, uint32_t src, uint16_t count)
{
    DiskTransferPacket* p;
    Regs r;

    p = &gDiskTransferPacket;
    p->size = 0x10;
    p->zero = 0;
    p->count = count;
    p->srcHi = 0;
    p->srcLo = src;

    canonize(&p->dstSegment, &p->dstOffset, (uint32_t)dst);

    r.esi = (uint32_t)p;
    r.eax = 0x4200;
    r.edx = drive;

    bios(0x13, &r);
}
