#include "../../include/out.h"
#include "../../include/keyboard.h"
#include "../../include/io.h"
#include "../../include/globals.h"

uint8_t modifiers = 0;

#include "../../include/globals.h"

unsigned char handle_normal(uint8_t scancode) {
    serial_print("Handling normal...\n");
    bool break_code = (scancode & 0x80);
    uint8_t code = scancode & 0x7F;

    if (code >= get_ScancodeMapSize()) return 0;
    unsigned char key = get_Scancode()[code];

    if (break_code) {
        if (key == LSHIFT) modifiers &= ~(1 << 1);
        else if (key == LCTRL)  modifiers &= ~(1 << 0);
        else if (key == LALT)   modifiers &= ~(1 << 3);
        else if (key == RSHIFT) modifiers &= ~(1 << 6);
        return 0;
    }

    if (key == LSHIFT) { modifiers |= (1 << 1); return 0; }
    else if (key == LCTRL)  { modifiers |= (1 << 0); return 0; }
    else if (key == LALT)   { modifiers |= (1 << 3); return 0; }
    else if (key == RSHIFT) { modifiers |= (1 << 6); return 0; }

    if (modifiers & (1 << 1) || modifiers & (1 << 6)) {
        serial_print("Shifted\n");
        return get_Scancode_Shifted()[code];
    } 
    
    else if (modifiers & (1 << 4)) {
        serial_print("Altgr\n");
        return get_Scancode_Altgr()[code];
    }

    return key;
}

unsigned char handle_extended(uint8_t scancode) {
    serial_print("Extended");
    bool break_code = (scancode & 0x80);
    uint8_t ext = scancode & 0x7F;

    if (break_code) {
        if (ext == 0x1D) modifiers &= ~(1 << 7); // RCTRL
        else if (ext == 0x38) modifiers &= ~(1 << 4); // RALT
        else if (ext == 0x5B) modifiers &= ~(1 << 2); // LSUPER
        else if (ext == 0x5C) modifiers &= ~(1 << 5); // RSUPER
        return 0;
    }

    switch (ext) {
        case 0x1D: modifiers |= (1 << 7); return 0; // RCTRL
        case 0x38: modifiers |= (1 << 4); return 0; // RALT
        case 0x5B: modifiers |= (1 << 2); return 0; // LSUPER
        case 0x5C: modifiers |= (1 << 5); return 0; // RSUPER
        
        // Navigation Keys
        case 0x48: return ARROWUP;
        case 0x50: return ARROWDOWN;
        case 0x4B: return ARROWLEFT;
        case 0x4D: return ARROWRIGHT;
        case 0x47: return STARTKEY;
        case 0x4F: return ENDKEY;
        case 0x52: return INSERTKEY;
        case 0x53: return DELETEKEY;
        
        default: return 0;
    }
}

static bool is_extended = false;

void keyboard_handler_callback() {
    serial_print("Callback\n");
    uint8_t scancode = inb(0x60);

    if (scancode == 0xE0) {
        is_extended = true;
        return; 
    }

    unsigned char result = 0;
    if (is_extended) {
        result = handle_extended(scancode);
        is_extended = false;
    } else {
        result = handle_normal(scancode);
    }

    if (result != 0) {
        kprint_char(result, 0x0F);
    }
}

uint8_t getModifiers() { return modifiers; }

void keyboard_init() {
    uint8_t mask = inb(0x21);
    mask &= ~(1 << 1);
    outb(0x21, mask);
    if (DEBUG_MODE) kprint("Keyboard initiated!\n");
}