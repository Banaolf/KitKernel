#include "../../../include/idtpic.h"
#include "../../../include/io.h"
#include "../../../include/out.h"
#include "../../../include/keyboard.h"
#include "../../../include/gdt.h"
#include "../../../include/kstring.h"

IDTEntry idt[256];
IDTR idtr;

void pic_init() {
	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	outb(0x21, 0x20);
	outb(0xA1, 0x28);

	outb(0x21, 0x04);
	outb(0xA1, 0x02);

	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	outb(0x21, 0xFD);
	outb(0xA1, 0xFF); 

	uint8_t mask = inb(0x21);
	mask &= ~(1 << 1); // Clear bit 1 to ENABLE IRQ1
	outb(0x21, mask);
}

void set_idt_gate(int n, uint64_t handler) {
	idt[n].offset_low = (uint16_t)(handler & 0xFFFF);
	idt[n].selector = 0x08;
	idt[n].ist = (IST_CURRENT_ALLOCATED-1);
	idt[n].type_attr = 0x8E;
	idt[n].offset_mid = (uint16_t)((handler >> 16) & 0xFFFF);
	idt[n].offset_high = (uint32_t)((handler >> 32) & 0xFFFFFFFF);
	idt[n].reserved = 0;
}

extern "C" uint64_t interrupt_handler_table[256];

extern "C" void common_interrupt_handler(interrupt_frame* frame) {
	serial_print("Interrupt Received: ");
	serial_print(fmtString("%i\n", (int)frame->interrupt_number).cstr());
	
	if (frame->interrupt_number == 33) {
		keyboard_handler_callback();
	}

	if (frame->interrupt_number >= 32 && frame->interrupt_number <= 47) {
		if (frame->interrupt_number >= 40) outb(0xA0, 0x20);
		outb(0x20, 0x20);
	}
}

extern "C" uint64_t interrupt_handler_table[256];

void idt_init() {
	for (int i = 0; i < 256; i++) {
		set_idt_gate(i, interrupt_handler_table[i]);
	}

	idtr.limit = (uint16_t)sizeof(idt) - 1;
	idtr.base = (uint64_t)&idt;
	asm volatile("lidt %0" : : "m"(idtr));
}