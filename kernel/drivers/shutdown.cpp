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