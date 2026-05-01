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
    serial_print("Broke!\n");

    kprint_times(COL_MAX, "-");
    
    serial_print(fmtString("Vector: %x\nErrCode: %x\nRIP: %x\nCS: %x\nRFLAGS: %x\nRBP %x\n", frame->interrupt_number, frame->error_code, frame->rip, frame->cs, frame->rflags, frame->rbp).cstr());
    kprintf("Vector: %x\nErrCode: %x\nRIP: %x\nCS: %x\nRFLAGS: %x\nRBP %x\n", frame->interrupt_number, frame->error_code, frame->rip, frame->cs, frame->rflags, frame->rbp);

    halt();
}

extern "C" void common_interrupt_handler(interrupt_frame* frame) {
    serial_print("Interrupt Received: ");
    serial_print(fmtString("%i\n", (int)frame->interrupt_number).cstr());
    
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