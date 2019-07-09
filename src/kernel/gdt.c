#include "kernel.h"

typedef struct {
    uint16_t    limitLo;
    uint16_t    baseLo;
    uint8_t     baseMd;
    uint8_t     access;
    uint8_t     limitHi:4;
    uint8_t     flags:4;
    uint8_t     baseHi;
} PACKED ALIGN(8) GdtEntry;

static const GdtEntry gGDT[] = {
    {0x0000, 0x0000, 0x00, 0x00, 0x0, 0x0, 0x00},
    {0x0000, 0x0000, 0x00, 0x98, 0x0, 0x2, 0x00},
    {0x0000, 0x0000, 0x00, 0x92, 0x0, 0x0, 0x00},
};

void gdt_init(void)
{
    gdt_load(gGDT, sizeof(gGDT) / sizeof(*gGDT));
}
