#include "kernel.h"

typedef struct {
    uint16_t offsetLo;
    uint16_t selector;
    unsigned ist:3;
    unsigned zero0:5;
    unsigned type:4;
    unsigned zero1:1;
    unsigned dpl:2;
    unsigned present:1;
    uint16_t offsetMd;
    uint32_t offsetHi;
    uint32_t zero2;
} PACKED ALIGN(8) IDTDescriptor;

typedef struct {
    uint16_t                size;
    const IDTDescriptor*    descriptors;
} PACKED ALIGN(16) IDT;

IDTDescriptor gIDTDescriptors[0x30];
IDT gIDT;

void idt_init(void)
{
    gIDT.size = sizeof(gIDTDescriptors) - 1;
    gIDT.descriptors = gIDTDescriptors;

    ASM ("lidt %0\r\n" :: "m"(gIDT));
}

void idt_register(uint8_t index, uint8_t ist, void* handler)
{
    uint64_t handlerAddr;

    handlerAddr = (uint64_t)handler;
    gIDTDescriptors[index].offsetLo = (handlerAddr & 0xffff);
    gIDTDescriptors[index].selector = GDT_CODE_RING0;
    gIDTDescriptors[index].ist = ist;
    gIDTDescriptors[index].zero0 = 0;
    gIDTDescriptors[index].type = DESC_TYPE_INTERRUPT;
    gIDTDescriptors[index].zero1 = 0;
    gIDTDescriptors[index].dpl = 0;
    gIDTDescriptors[index].present = 1;
    gIDTDescriptors[index].offsetMd = (handlerAddr >> 16) & 0xffff;
    gIDTDescriptors[index].offsetHi = (handlerAddr >> 32);
    gIDTDescriptors[index].zero2 = 0;
}
