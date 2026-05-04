#include "../../../include/idtpic.h"
#include "../../../include/out.h"
#include "../../../include/keyboard.h"
#include "../../../include/kstring.h"
#include "../../../include/pit.h"

IDTEntry idt[256];
IDTR idtr;

extern "C" uint64_t interrupt_handler_table[256];

void set_idt_gate(int n, uint64_t handler) {
    int istn = 0;
    if (n == 8 || n == 13 || n == 14) istn = 1;
    idt[n].offset_low = (uint16_t)(handler & 0xFFFF);
    idt[n].selector = 0x08;
    idt[n].ist = istn;
    idt[n].type_attr = 0x8E;
    idt[n].offset_mid = (uint16_t)((handler >> 16) & 0xFFFF);
    idt[n].offset_high = (uint32_t)((handler >> 32) & 0xFFFFFFFF);
    idt[n].reserved = 0;
}

void breaks(interrupt_frame* frame) {
    asm volatile("cli"); 
    
    serial_print("!!! BROKE !!!\n");
    
    char buffer[19];
    kprintln(uint_to_str(frame->interrupt_number, buffer));
    char buffer2[19];
    kprintln(uint_to_str(frame->error_code, buffer2));
    char buffer3[19];
    kprintln(uint_to_hex(frame->rip, buffer3));
    char buffer4[19];
    kprintln(uint_to_hex(frame->cs, buffer4));
    char buffer5[19];
    kprintln(uint_to_hex(frame->rsi, buffer5));
    char buffer6[19];
    kprintln(uint_to_hex(frame->rflags, buffer6));
    char buffer7[19];
    kprintln(uint_to_hex(frame->ss, buffer7));

    while(1) {
        asm volatile("hlt");
    }
}

extern "C" void common_interrupt_handler(interrupt_frame* frame) {
    switch(frame->interrupt_number) {
        case 3:
            break;
        case 0: case 1: case 2: case 4: case 5: case 6: case 7: case 9: case 10:
        case 11: case 12: case 13: case 14: case 15: case 16: case 17:
        case 18: case 19: case 20: case 21: case 22: case 23: case 24:
        case 25: case 26: case 27: case 28: case 29: case 30: case 31: 
            breaks(frame); break;
        case 32: pit_handler(); break;
        case 33: keyboard_handler_callback(); outb(0x20, 0x20); break;
        default:
            serial_print("Received unknown interrupt.\n");
            break;
    }
}

void idt_init() {
    for (int i = 0; i < 256; i++) {
        set_idt_gate(i, interrupt_handler_table[i]);
    }

    idtr.limit = (uint16_t)sizeof(idt) - 1;
    idtr.base = (uint64_t)&idt;
    asm volatile("lidt %0" : : "m"(idtr));
}