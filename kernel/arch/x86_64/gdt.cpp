#include "../../../include/gdt.h"
GDTEntry gdt[7];
GDTDescriptor gdt_ptr;

extern "C" void load_gdt(GDTDescriptor*);

void install_tss(uint64_t tss_addr) {
    uint32_t limit = sizeof(TSS) - 1;
    gdt[5].limit_low   = limit & 0xFFFF;
    gdt[5].base_low    = tss_addr & 0xFFFF;
    gdt[5].base_middle = (tss_addr >> 16) & 0xFF;
    gdt[5].access      = 0x89;
    gdt[5].attributes  = ((limit >> 16) & 0x0F);
    gdt[5].base_high   = (tss_addr >> 24) & 0xFF;

    uint64_t* upper_base = (uint64_t*)&gdt[6];
    *upper_base = (tss_addr >> 32) & 0xFFFFFFFF;
}

void init_gdt() {
    // Null Descriptor
    gdt[0] = {0, 0, 0, 0, 0, 0};

    // Kernel Code Segment
    gdt[1].limit_low   = 0;
    gdt[1].base_low    = 0;
    gdt[1].base_middle = 0;
    gdt[1].access      = 0x9A;
    gdt[1].attributes  = 0x20; 
    gdt[1].base_high   = 0;

    // Kernel Data Segment
    gdt[2].limit_low   = 0;
    gdt[2].base_low    = 0;
    gdt[2].base_middle = 0;
    gdt[2].access      = 0x92;
    gdt[2].attributes  = 0x00;
    gdt[2].base_high   = 0;

    //User Code Segment
    gdt[3].limit_low   = 0;
    gdt[3].base_low    = 0;
    gdt[3].base_middle = 0;
    gdt[3].access      = 0xFA;
    gdt[3].attributes  = 0x20;
    gdt[3].base_high   = 0;

    //User Data Segment
    gdt[4].limit_low   = 0;
    gdt[4].base_low    = 0;
    gdt[4].base_middle = 0;
    gdt[4].access      = 0xF2;
    gdt[4].attributes  = 0x00;
    gdt[4].base_high   = 0;

    //TSS
    install_tss((uint64_t)&kernel_tss);

    // Setup the pointer
    gdt_ptr.size = sizeof(gdt) - 1;
    gdt_ptr.offset = (uint64_t)&gdt;

    // Load
    load_gdt(&gdt_ptr);
}