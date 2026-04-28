#include <stdint.h>
#include "../../include/out.h"
#include "../../include/gdt.h"
#include "../../include/keyboard.h"
#include "../../include/idtpic.h"
#include "../../include/heap.h"
#include "../../include/pmm.h"
#include "../../include/io.h"

extern uint64_t g_pml4_phys;

extern "C" void kernel_main(uint64_t magic, uint64_t* multiboot_address) {
	serial_init();
	serial_print("Welcome!\n");
	init_gdt();
	serial_print("Initted GDT\n");
	idt_init();
	serial_print("Initted IDT\n");
	pic_init();
	serial_print("Initted PIC\n");

	pmm_init(multiboot_address);
	serial_print("Initted PMM\n");
	paging_init();
	serial_print("Initted Paging\n");
	heap_init(g_pml4_phys);
	serial_print("Initted Heap\n");

	tss_init();
	serial_print("Initted TSS(memory)\n");

	keyboard_init();
	serial_print("Initted Keyboard\n");
	asm volatile("sti");
	serial_print("Ended Initiating!\n");

	if (magic != 0x36d76289) {
		serial_print("Magic number does not match!\n");
		halt();
	}

	serial_print("Magic number is correct.\n");
	kprint("Welcome to Kit Kernel!\n");
	kprint_char('>', 0x0F);

	while (true) {
		asm volatile("hlt");
	}
}