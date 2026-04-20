#include <stdint.h>
#include "../../include/out.h"
#include "../../include/gdt.h"
#include "../../include/keyboard.h"
#include "../../include/idtpic.h"
#include "../../include/heap.h"
#include "../../include/pmm.h"
#include "../../include/io.h"

extern "C" void kernel_main(uint64_t magic, uint64_t* multiboot_address) {
	serial_print("Welcome to Kit Kernel!");
	init_gdt();
	idt_init();
	pic_init();

	pmm_init(multiboot_address);
	multiboot_tag_framebuffer* fb = get_fb_tag(multiboot_address);
	uint64_t* pml4 = paging_init();
	heap_init((uint64_t)pml4);

	keyboard_init();
	asm volatile("sti");
	serial_print("Ended Initiating...");

	if (magic != 0x36d76289) {
		serial_print("Magic number does not match!");
		halt();
	}

	serial_print("Magic number is correct.");
	kprint("Welcome to Kit Kernel!\n");
	kprint_char('>', 0x0F);

	while (true) {
		asm volatile("hlt");
	}
}