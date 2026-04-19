ENTRY(_start)

SECTIONS
{
    /* GRUB usually loads kernels at the 1MB mark */
    . = 1M;

    .text :
    {
        /* Ensure multiboot header is at the very beginning */
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
}