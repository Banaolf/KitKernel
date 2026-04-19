#include <stdint.h>
#include "../../include/out.h"
#include "../../include/gdt.h"
#include "../../include/keyboard.h"
#include "../../include/idtpic.h"
#include "../../include/io.h"

extern "C" void kernel_main(uint64_t magic, uint64_t multiboot_address) {
	serial_print("Welcome to Kit Kernel!");
	init_gdt();
	idt_init();
	pic_init();
	keyboard_init();
	asm volatile("sti");
	serial_print("Ended Initiating...");

	if (magic != 0x36d76289) {
		serial_print("Magic number does not match!");
		halt();
	}

	serial_print("Magic number is correct.");
	kprint("Welcome to Kit Kernel!\n");
	kprint_char('>', 0x70);

	halt();
}