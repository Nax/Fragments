OUTPUT_FORMAT("binary")
SECTIONS
{
    .text 0x0600 : { *(.text) }
    /DISCARD/ : { *(*) }
}
