#include <stdint.h>
#include "../../include/out.h"
#include "../../include/io.h"

extern "C" void kernel_main(uint64_t magic, uint64_t multiboot_address) {
    if (magic != 0x36d76289) {
        serial_print("Magic number does not match!");
        halt();
    }
    serial_print("Test.");
    kprint("Welcome to Kit Kernel!");
    halt();
}