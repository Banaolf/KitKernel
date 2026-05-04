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
	serial_print("Magic number is correct.\n");
	serial_print("Initiating!\n");
	init_gdt();
	serial_print("Initiated GDT\n");
	idt_init();
	serial_print("Initiated IDT\n");
	pic_init();
	serial_print("Initiated PIC\n");

	pmm_init(multiboot_address);
	serial_print("Initiated PMM\n");
	paging_init();
	serial_print("Initiated Paging\n");
	heap_init(g_pml4_phys);
	serial_print("Initiated Heap\n");

	tss_init();
	serial_print("Initiated TSS(memory)\n");

	keyboard_init();
	serial_print("Initiated Keyboard\n");
	pit_init(100);
	serial_print("Initiated PIT(Programable Interval Timer)\n");
	asm volatile("sti");
	serial_print("Ended Initiating!\n");

	kprint("Welcome to Kit Kernel!\n");
	kprint_char('>', 0x0F);

	while (1) {
		if (newlined) {
			newlined = false;
			serial_print("Breakpoint 1\n"); //THIS DOES NOT PRINT!
			String s = kgets();
			serial_print("Breakpoint 2\n");
			shmain(s.cstr());
			serial_print("Breakpoint 3\n");
			kprint_char('>', 0x0F); //DOES NOT GET HERE!
			serial_print("Breakpoint 4\n");
		}
		serial_print("Breakpoint 5\n");

		khas_interrupted = false;
		serial_print("Breakpoint 6\n"); //GET'S HERE!
		while (!khas_interrupted) {
			asm volatile("hlt");
		}
	}

	halt();
}
}