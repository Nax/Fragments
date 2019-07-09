OUTPUT_FORMAT("binary")
SECTIONS
{
    . = 0x7c00;

    .text.start : ALIGN(0x10) { *(.text.start) }
    .data16 : ALIGN(0x10) { *(.data16) }
    .text16 : ALIGN(0x10) { *(.text16) }
    .text : ALIGN(0x10) { *(.text) }
    .data : ALIGN(0x10) { *(.data) }
    .rodata : ALIGN(0x10) { *(.rodata*) }
    .bss : ALIGN(0x10) { *(.bss*) *(COMMON*) }
    /DISCARD/ : { *(*) }
}
