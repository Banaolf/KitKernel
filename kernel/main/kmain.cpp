#include <stdint.h>
#include "../../include/out.h"
#include "../../include/gdt.h"
#include "../../include/keyboard.h"
#include "../../include/idtpic.h"
#include "../../include/heap.h"
#include "../../include/pmm.h"
#include "../../include/shell.h"
#include "../../include/pit.h"
#include "../../include/io.h"

extern uint64_t g_pml4_phys;
extern "C" {
void* __dso_handle = 0;
int __cxa_atexit(void (*f)(void *), void *p, void *d) { (void)f; (void)p; (void)d; return 0; }
void kernel_main(uint64_t magic, uint64_t* multiboot_address) {
	serial_init();
	if (magic != 0x36d76289) {
		serial_print("Magic number does not match!\n");
		halt();
	}
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
	pit_init(100);
	serial_print("Initted PIT(Programable Interval Timer)\n");
	asm volatile("sti");
	serial_print("Ended Initiating!\n");

	serial_print("Magic number is correct.\n");
	kprint("Welcome to Kit Kernel!\n");
	kprint_char('>', 0x0F);

	while (1) {
		if (newlined) {
			newlined = false;
			String s = kgets();
			shmain(s.cstr());
			kprint_char('>', 0x0F);
		}

		khas_interrupted = false; 
		while (!khas_interrupted) {
			asm volatile("hlt");
		}
	}

	halt();
}
}