#include "kernel.h"

typedef struct {
    uint16_t    limitLo;
    uint16_t    baseLo;
    uint8_t     baseMd;
    unsigned    accessed:1;
    unsigned    rw:1;
    unsigned    dc:1;
    unsigned    execute:1;
    unsigned    segment:1;
    unsigned    level:2;
    unsigned    present:1;
    uint8_t     limitHi:4;
    unsigned    zero:1;
    unsigned    longmode:1;
    unsigned    size:1;
    unsigned    granularity:1;
    uint8_t     baseHi;
} PACKED GdtDescriptor;

typedef struct
{
    uint16_t    limit0;
    uint16_t    base0;
    uint8_t     base1;
    unsigned    type:4;
    unsigned    zero0:1;
    unsigned    dpl:2;
    unsigned    present:1;
    unsigned    limit1:4;
    unsigned    avl:1;
    unsigned    zero1:2;
    unsigned    granularity:1;
    uint8_t     base2;
    uint32_t    base3;
    uint32_t    zero2;
} PACKED TssDescriptor;

typedef union
{
    TssDescriptor       tss;
    GdtDescriptor       gdt[2];
} PACKED ALIGN(16) AnyDescriptor;

static AnyDescriptor gDescriptors[32];

static GdtDescriptor* _gdt(uint16_t index)
{
    index /= 8;

    if (!(index & 1))
        return gDescriptors[index / 2].gdt + 0;
    else
        return gDescriptors[index / 2].gdt + 1;
}

static TssDescriptor* _tss(uint16_t index)
{
    index /= 16;

    return &gDescriptors[index].tss;
}

static void _gdtMakeCode(uint16_t index, int ring)
{
    GdtDescriptor* desc;

    desc = _gdt(index);
    desc->limitLo = 0;
    desc->baseLo = 0;
    desc->baseMd = 0;
    desc->accessed = 0;
    desc->rw = 0;
    desc->dc = 0;
    desc->execute = 1;
    desc->segment = 1;
    desc->level = ring;
    desc->present = 1;
    desc->limitHi = 0;
    desc->zero = 0;
    desc->longmode = 1;
    desc->size = 0;
    desc->granularity = 0;
    desc->baseHi = 0;
}

void gdt_init(void)
{
    _gdtMakeCode(GDT_CODE_RING0, 0);
    gdt_load(gDescriptors, sizeof(gDescriptors) - 1, GDT_CODE_RING0);
}

void gdt_describe_tss(uint16_t index, Tss* tss, size_t size)
{
    TssDescriptor* desc;

    desc = _tss(index);
    desc->limit0 = size & 0xffff;
    desc->base0 = (uint64_t)tss & 0xffff;
    desc->base1 = ((uint64_t)tss >> 16) & 0xff;
    desc->type = DESC_TYPE_TSS;
    desc->zero0 = 0;
    desc->dpl = 0;
    desc->present = 1;
    desc->limit1 = (size >> 16) & 0xff;
    desc->avl = 0;
    desc->zero1 = 0;
    desc->granularity = 0;
    desc->base2 = ((uint64_t)tss >> 24) & 0xff;
    desc->base3 = ((uint64_t)tss >> 32) & 0xffffffff;
    desc->zero2 = 0;
}
