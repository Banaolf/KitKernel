#include "../../include/heap.h"
#include "../../include/out.h"
#include "../../include/pmm.h"

uint64_t g_pml4_phys = 0;

void map_page(uint64_t pml4_phys, uint64_t virt, uint64_t phys, uint64_t flags) {
    page_table* current_table = (page_table*)pml4_phys;
    uint64_t indices[4] = { pml4_idx(virt), pdpt_idx(virt), pd_idx(virt), pt_idx(virt) };

    for (int i = 0; i < 3; i++) {
        uint64_t entry = current_table->entries[indices[i]];
        if (!(entry & PRESENT)) {
            uint64_t new_table = (uint64_t)kmalloc_page(4096);
            zero_page((void*)new_table);
            current_table->entries[indices[i]] = new_table | PRESENT | WRITABLE;
            current_table = (page_table*)new_table;
        } else {
            current_table = (page_table*)get_addr(entry);
        }
    }
    current_table->entries[indices[3]] = phys | flags | PRESENT;
    invlpg(virt);
}

void paging_init() {
    serial_print("Allocating pml4...\n");
    uint64_t* local_pml4 = (uint64_t*)kmalloc_page(4096);
    
    if (!local_pml4) {
        serial_print("Pml4 couldn't be allocated!\n");
        while(1) { asm("hlt"); } 
    }

    serial_print("Zeroing pml4\n");
    zero_page(local_pml4);

    serial_print("Getting upper memory\n");
    uint64_t upper = get_memupper();
    serial_print("Mapping PML4\n");
    for (uint64_t addr = 0; addr < upper; addr += 4096) {
        map_page((uint64_t)local_pml4, addr, addr, WRITABLE);
    }

    serial_print("Assigning g_mpl4_phys\n");
    g_pml4_phys = (uint64_t)local_pml4;
    
    serial_print("Making final assembly stub\n");
    asm volatile("mov %0, %%cr3" : : "r"(g_pml4_phys) : "memory");
}

void heap_init(uint64_t pml4_phys) {
    uint64_t heap_virtual_start = 0x100000000; 

    serial_print("Mapping pages...");
    for (uint64_t i = 0; i < 1024 * 1024; i += 4096) {
        serial_print("Allocating page...");
        void* phys = kmalloc_page(4096);
        map_page(pml4_phys, heap_virtual_start + i, (uint64_t)phys, WRITABLE);
    }
}