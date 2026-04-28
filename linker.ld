ENTRY(_start)

SECTIONS
{
    . = 1M;
    _kernel_start = .;
    
    .text :
    {
        KEEP(*(.multiboot_header))
        *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
    }
    _kernel_end = .;
}