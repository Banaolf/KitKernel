#include "../../../include/gdt.h"
GDTEntry gdt[3];
GDTDescriptor gdt_ptr;

extern "C" void load_gdt(GDTDescriptor*);

void init_gdt() {
    // 1. Null Descriptor
    gdt[0] = {0, 0, 0, 0, 0, 0};

    // 2. Kernel Code Segment (64-bit)
    // Access: 0x9A (Present, Ring 0, Code, Executable/Readable)
    // Flags:  0x20 (Long Mode bit set)
    gdt[1].limit_low   = 0;
    gdt[1].base_low    = 0;
    gdt[1].base_middle = 0;
    gdt[1].access      = 0x9A;
    gdt[1].attributes  = 0x20; 
    gdt[1].base_high   = 0;

    // 3. Kernel Data Segment (64-bit)
    // Access: 0x92 (Present, Ring 0, Data, Writable)
    gdt[2].limit_low   = 0;
    gdt[2].base_low    = 0;
    gdt[2].base_middle = 0;
    gdt[2].access      = 0x92;
    gdt[2].attributes  = 0x00;
    gdt[2].base_high   = 0;

    // 4. Setup the pointer
    gdt_ptr.size = sizeof(gdt) - 1;
    gdt_ptr.offset = (uint64_t)&gdt;

    // 5. Load it!
    load_gdt(&gdt_ptr);
}