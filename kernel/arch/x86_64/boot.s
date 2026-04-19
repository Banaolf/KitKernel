; --- Multiboot 2 Header ---
section .multiboot_header
header_start:
    dd 0xe85250d6                ; Magic number (multiboot 2)
    dd 0                         ; Architecture 0 (protected mode i386)
    dd header_end - header_start ; Header length
    ; Checksum
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

    ; End tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:

section .text
bits 32
global _start

_start:
    mov esp, stack_top

    ; 1. Save multiboot info
    mov edi, eax
    mov esi, ebx

    ; 2. Check for Long Mode support
    call check_multiboot
    call check_cpuid
    call check_long_mode

    ; 3. Setup Paging
    call setup_page_tables
    call enable_paging

    ; 4. Load 64-bit GDT
    lgdt [gdt64.pointer]

    ; 5. Far jump to 64-bit Long Mode
    jmp gdt64.code:long_mode_start

; --- Compatibility Checks ---
check_multiboot:
    cmp eax, 0x36d76289
    jne .no_multiboot
    ret
.no_multiboot:
    mov al, "0"
    jmp error

check_cpuid:
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    cmp eax, ecx
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "1"
    jmp error

check_long_mode:
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb .no_long_mode
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz .no_long_mode
    ret
.no_long_mode:
    mov al, "2"
    jmp error

error:
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x4f3a4f52
    mov  byte [0xb8008], al
    hlt

; --- Paging Setup ---
setup_page_tables:
    ; Map first P4 entry to P3 table
    mov eax, p3_table
    or eax, 0b11 ; present + writable
    mov [p4_table], eax

    ; Map first P3 entry to P2 table
    mov eax, p2_table
    or eax, 0b11 ; present + writable
    mov [p3_table], eax

    ; Map each P2 entry to a huge 2MB page
    mov ecx, 0 ; counter
.map_p2_table:
    ; Map ecx-th P2 entry to a 2MB page starting at (2MB x ecx)
    mov eax, 0x200000 ; 2MB
    mul ecx
    or eax, 0b10000011 ; present + writable + huge
    mov [p2_table + ecx * 8], eax

    inc ecx
    cmp ecx, 512 ; whole table mapped
    jne .map_p2_table
    ret

enable_paging:
    ; Pass page table location to CPU
    mov eax, p4_table
    mov cr3, eax

    ; Enable PAE (Physical Address Extension) flag in cr4
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Set the long mode bit in the EFER MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable paging in cr0
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax
    ret

; --- GDT Definition ---
section .rodata
gdt64:
    dq 0 ; zero entry
.code: equ $ - gdt64
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

; --- 64-Bit Entry Point ---
bits 64
long_mode_start:
    ; Clear segment registers
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Call Kernel
    extern kernel_main
    call kernel_main

    hlt

; --- Reserve memory for tables and stack ---
section .bss
align 4096
p4_table:
    resb 4096
p3_table:
    resb 4096
p2_table:
    resb 4096
stack_bottom:
    resb 4096 * 4
stack_top: