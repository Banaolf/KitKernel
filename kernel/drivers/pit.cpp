#include "../../include/pit.h"
#include "../../include/io.h"

typedef unsigned char uint8_t;

static unsigned long pit_ticks = 0;

void pit_init(uint32_t frequency) {
    uint32_t divisor = INTERNAL_CLOCK_RUN_RATE / frequency;

    outb(0x43, 0x36);

    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor << 8) & 0xFF));
}

void pit_handler() {
    pit_ticks++;
    outb(0x20, 0x20);
}

void sleep(uint32_t time_in_miliseconds) {
    unsigned long end = pit_ticks + time_in_miliseconds;
    while(pit_ticks > end) {asm volatile("hlt");}
}