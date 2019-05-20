OUTPUT_FORMAT("binary")
SECTIONS
{
    .text 0x0400 : { *(.text) }
    /DISCARD/ : { *(*) }
}
