#include "kernel.h"

typedef struct {
    uint16_t offsetLo;
    uint16_t selector;
    uint8_t ist;
    uint8_t attr;
    uint16_t offsetMd;
    uint32_t offsetHi;
    uint32_t zero;
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

void idt_register(uint8_t index, uint16_t selector, uint8_t ist, void* handler)
{
    uint64_t handlerAddr;

    handlerAddr = (uint64_t)handler;
    gIDTDescriptors[index].offsetLo = (handlerAddr & 0xffff);
    gIDTDescriptors[index].selector = selector;
    gIDTDescriptors[index].ist = ist;
}
