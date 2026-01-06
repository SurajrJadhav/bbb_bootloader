ENTRY(_start)

MEMORY
{
  sram (rwx) : ORIGIN = 0x402F0400, LENGTH = 0x0000FC00
}

SECTIONS
{
    .text :
    {
        *(.text*)
        *(.rodata*)
    } > sram

    .data :
    {
        *(.data*)
    } > sram

    .bss (NOLOAD) :
    {
        *(.bss*)
        *(COMMON)
    } > sram

    /* Simple stack at the end of SRAM */
    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        . = . + 0x1000; /* 4KB Stack size */
        _stack_top = .;
    } > sram
}
