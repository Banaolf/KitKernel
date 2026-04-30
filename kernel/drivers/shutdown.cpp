#include "../../include/shutdown.h"
#include "../../include/io.h"
#include "../../include/out.h"

//Shut down the system
void shutdown() {
    kprint("Shutting down... (QEMU)");
    outw(0x604, 0x2000); //QEMU fallback.

    halt();
}

//Restart the system
void restart() {
    outb(0x64, 0xFE);
    // If that didn't work, triple fault
    asm volatile("cli");
    asm volatile("lidt 0");
    asm volatile("int $0");
}

//Reboots right back into the firmware
void legacy_tofirmware() {
    uint16_t* warm_reset_vector = (uint16_t*)0x0472;
    *warm_reset_vector = 0x1234;
    
    uint8_t good = 0x02;
    while (good & 0x02) {
        good = inb(0x64);
    }
    outb(0x64, 0xFE);
    
    asm volatile("cli");
    asm volatile("int $3"); 
}