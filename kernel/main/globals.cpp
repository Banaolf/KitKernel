#include "../../include/globals.h"
#include "../../include/keyboard.h"
#include <stdint.h>

static uint8_t scancodeid = 0x00;

uint8_t get_ScancodeMapID() { return scancodeid; }


static const unsigned char scancode_map_00[] = { //FOR KEYBOARD Spanish(Spain). More support will come after
    0,          // 0x00
    ESC,        // 0x01
    '1','2','3','4','5','6','7','8','9','0','\'',0xAD, // 0x02-0x0D (0xAD=¡)
    '\b',       // 0x0E
    '\t',       // 0x0F
    'q','w','e','r','t','y','u','i','o','p','`','+', // 0x10-0x1B
    '\n',       // 0x1C
    LCTRL,      // 0x1D
    'a','s','d','f','g','h','j','k','l',0xA4,'\'', // 0x1E-0x28 (0xA4=ñ, ´ approximated as ')
    0xA7,       // 0x29 º (codepage 437)
    LSHIFT,     // 0x2A
    0x87,        // 0x2B
    'z','x','c','v','b','n','m',',','.','-', // 0x2C-0x35
    RSHIFT,     // 0x36
    '*',        // 0x37
    LALT,       // 0x38
    ' ',        // 0x39
    CAPSLOCK,   // 0x3A
    FUNCTION1, FUNCTION2, FUNCTION3, FUNCTION4, FUNCTION5,
    FUNCTION6, FUNCTION7, FUNCTION8, FUNCTION9, FUNCTION10, // 0x3B-0x44
    NUMLOCK,    // 0x45
    0,          // 0x46 scroll lock
    '7',        // 0x47 numpad 7
    '8',        // 0x48 numpad 8
    '9',        // 0x49 numpad 9
    '-',        // 0x4A numpad -
    '4',        // 0x4B numpad 4
    '5',        // 0x4C numpad 5
    '6',        // 0x4D numpad 6
    '+',        // 0x4E numpad +
    '1',        // 0x4F numpad 1
    '2',        // 0x50 numpad 2
    '3',        // 0x51 numpad 3
    '0',        // 0x52 numpad 0
    '.',        // 0x53 numpad .
    0,          // 0x54 Unknown/Not present
    0,          //0x55 Unknown/Not present
    '<'         //0x56 <
};

static const unsigned char scancode_map_shift_00[] = { //FOR KEYBOARD Spanish(Spain). More support will come after
    0,          // 0x00
    ESC,        // 0x01
    '!','"',0xFA,'$','%','&','/','(',')','=','?',0xA8, // 0x02-0x0D (0xAD=¡)
    '\b',       // 0x0E
    '\t',       // 0x0F
    'Q','W','E','R','T','Y','U','I','O','P','^','*', // 0x10-0x1B
    '\n',       // 0x1C
    LCTRL,      // 0x1D
    'A','S','D','F','G','H','J','K','L',0xA5,'"', // 0x1E-0x28 (0xA4=ñ, ¨ approximated as ")
    0xA6,       // 0x29 ª (codepage 437)
    LSHIFT,     // 0x2A
    0x80,        // 0x2B
    'Z','X','C','V','B','N','M',';',':','_', // 0x2C-0x35
    RSHIFT,     // 0x36
    '*',        // 0x37
    LALT,       // 0x38
    ' ',        // 0x39
    CAPSLOCK,   // 0x3A
    FUNCTION1, FUNCTION2, FUNCTION3, FUNCTION4, FUNCTION5,
    FUNCTION6, FUNCTION7, FUNCTION8, FUNCTION9, FUNCTION10, // 0x3B-0x44
    NUMLOCK,    // 0x45
    0,          // 0x46 scroll lock
    '7',        // 0x47 numpad 7
    '8',        // 0x48 numpad 8
    '9',        // 0x49 numpad 9
    '-',        // 0x4A numpad -
    '4',        // 0x4B numpad 4
    '5',        // 0x4C numpad 5
    '6',        // 0x4D numpad 6
    '+',        // 0x4E numpad +
    '1',        // 0x4F numpad 1
    '2',        // 0x50 numpad 2
    '3',        // 0x51 numpad 3
    '0',        // 0x52 numpad 0
    '.',        // 0x53 numpad .
    0,
    0,
    '>'
};

static const unsigned char scancode_map_altgr_00[] = {
    0,          // 0x00
    ESC,        // 0x01
    '|','@','#','~','5',0xAA,'7','8','9','0','\'', 0xAD, // 0x02-0x0D (0xAD=¡)
    '\b',       // 0x0E
    '\t',       // 0x0F
    'q','w','e','r','t','y','u','i','o','p','[',']', // 0x10-0x1B
    '\n',       // 0x1C
    LCTRL,      // 0x1D
    'a','s','d','f','g','h','j','k','l',0xA4,'{', // 0x1E-0x28 (0xA4=ñ, ´ approximated as ')
    0xA7,       // 0x29 º (codepage 437)
    LSHIFT,     // 0x2A
    '}',        // 0x2B
    'z','x','c','v','b','n','m',',','.','-', // 0x2C-0x35
    RSHIFT,     // 0x36
    '*',        // 0x37
    LALT,       // 0x38
    ' ',        // 0x39
    CAPSLOCK,   // 0x3A
    FUNCTION1, FUNCTION2, FUNCTION3, FUNCTION4, FUNCTION5,
    FUNCTION6, FUNCTION7, FUNCTION8, FUNCTION9, FUNCTION10, // 0x3B-0x44
    NUMLOCK,    // 0x45
    0,          // 0x46 scroll lock
    '7',        // 0x47 numpad 7
    '8',        // 0x48 numpad 8
    '9',        // 0x49 numpad 9
    '-',        // 0x4A numpad -
    '4',        // 0x4B numpad 4
    '5',        // 0x4C numpad 5
    '6',        // 0x4D numpad 6
    '+',        // 0x4E numpad +
    '1',        // 0x4F numpad 1
    '2',        // 0x50 numpad 2
    '3',        // 0x51 numpad 3
    '0',        // 0x52 numpad 0
    '.',        // 0x53 numpad .
    0,
    0,
    '<'
};

// 0x01 - Spanish Latin America
static const unsigned char scancode_map_01[] = {
    0,          // 0x00
    ESC,        // 0x01
    '1','2','3','4','5','6','7','8','9','0','\'',0xAD, // 0x02-0x0D (¡)
    '\b',       // 0x0E
    '\t',       // 0x0F
    'q','w','e','r','t','y','u','i','o','p','`','+', // 0x10-0x1B
    '\n',       // 0x1C
    LCTRL,      // 0x1D
    'a','s','d','f','g','h','j','k','l',0xA4,'\'', // 0x1E-0x28 (ñ, ´)
    0xA7,       // 0x29 º
    LSHIFT,     // 0x2A
    '<',        // 0x2B
    'z','x','c','v','b','n','m',',','.','-', // 0x2C-0x35
    RSHIFT,     // 0x36
    '*',        // 0x37
    LALT,       // 0x38
    ' ',        // 0x39
    CAPSLOCK,   // 0x3A
    FUNCTION1, FUNCTION2, FUNCTION3, FUNCTION4, FUNCTION5,
    FUNCTION6, FUNCTION7, FUNCTION8, FUNCTION9, FUNCTION10,
    NUMLOCK,    // 0x45
    0,          // 0x46
    '7','8','9', // 0x47-0x49
    '-',        // 0x4A
    '4','5','6', // 0x4B-0x4D
    '+',        // 0x4E
    '1','2','3', // 0x4F-0x51
    '0',        // 0x52
    '.',        // 0x53
    0,0,        // 0x54-0x55
    '<'         // 0x56
};

static const unsigned char scancode_map_shift_01[] = {
    0,          // 0x00
    ESC,        // 0x01
    '!','"','#','$','%','&','/','(',')','=','?',0xA8, // 0x02-0x0D (¿)
    '\b',       // 0x0E
    '\t',       // 0x0F
    'Q','W','E','R','T','Y','U','I','O','P','`','+', // 0x10-0x1B
    '\n',       // 0x1C
    LCTRL,      // 0x1D
    'A','S','D','F','G','H','J','K','L',0xA5,'"', // 0x1E-0x28 (Ñ)
    0xA6,       // 0x29 ª
    LSHIFT,     // 0x2A
    '>',        // 0x2B
    'Z','X','C','V','B','N','M',';',':','_', // 0x2C-0x35
    RSHIFT,     // 0x36
    '*',        // 0x37
    LALT,       // 0x38
    ' ',        // 0x39
    CAPSLOCK,   // 0x3A
    FUNCTION1, FUNCTION2, FUNCTION3, FUNCTION4, FUNCTION5,
    FUNCTION6, FUNCTION7, FUNCTION8, FUNCTION9, FUNCTION10,
    NUMLOCK,    // 0x45
    0,          // 0x46
    '7','8','9',
    '-',
    '4','5','6',
    '+',
    '1','2','3',
    '0',
    '.',
    0,0,
    '>'
};

static const unsigned char scancode_map_altgr_01[] = {
    0,          // 0x00
    ESC,        // 0x01
    0,'@','#','$',0,0,'{','[',']','}','\\',0, // 0x02-0x0D
    '\b',       // 0x0E
    '\t',       // 0x0F
    0,0,0,0,0,0,0,0,0,0,0,0, // 0x10-0x1B
    '\n',       // 0x1C
    LCTRL,      // 0x1D
    0,0,0,0,0,0,0,0,0,0,0, // 0x1E-0x28
    0,          // 0x29
    LSHIFT,     // 0x2A
    0,          // 0x2B
    0,0,0,0,0,0,0,0,0,0, // 0x2C-0x35
    RSHIFT,     // 0x36
    '*',        // 0x37
    LALT,       // 0x38
    ' ',        // 0x39
    CAPSLOCK,   // 0x3A
    FUNCTION1, FUNCTION2, FUNCTION3, FUNCTION4, FUNCTION5,
    FUNCTION6, FUNCTION7, FUNCTION8, FUNCTION9, FUNCTION10,
    NUMLOCK,    // 0x45
    0,          // 0x46
    '7','8','9',
    '-',
    '4','5','6',
    '+',
    '1','2','3',
    '0',
    '.',
    0,0,
    0
};

// 0x02 - English US
static const unsigned char scancode_map_02[] = {
    0,          // 0x00
    ESC,        // 0x01
    '1','2','3','4','5','6','7','8','9','0','-','=', // 0x02-0x0D
    '\b',       // 0x0E
    '\t',       // 0x0F
    'q','w','e','r','t','y','u','i','o','p','[',']', // 0x10-0x1B
    '\n',       // 0x1C
    LCTRL,      // 0x1D
    'a','s','d','f','g','h','j','k','l',';','\'','`', // 0x1E-0x29
    LSHIFT,     // 0x2A
    '\\',       // 0x2B
    'z','x','c','v','b','n','m',',','.','/', // 0x2C-0x35
    RSHIFT,     // 0x36
    '*',        // 0x37
    LALT,       // 0x38
    ' ',        // 0x39
    CAPSLOCK,   // 0x3A
    FUNCTION1, FUNCTION2, FUNCTION3, FUNCTION4, FUNCTION5,
    FUNCTION6, FUNCTION7, FUNCTION8, FUNCTION9, FUNCTION10,
    NUMLOCK,    // 0x45
    0,          // 0x46
    '7','8','9',
    '-',
    '4','5','6',
    '+',
    '1','2','3',
    '0',
    '.',
    0,0,
    0           // 0x56 not used on US layout
};

static const unsigned char scancode_map_shift_02[] = {
    0,          // 0x00
    ESC,        // 0x01
    '!','@','#','$','%','^','&','*','(',')','_','+', // 0x02-0x0D
    '\b',       // 0x0E
    '\t',       // 0x0F
    'Q','W','E','R','T','Y','U','I','O','P','{','}', // 0x10-0x1B
    '\n',       // 0x1C
    LCTRL,      // 0x1D
    'A','S','D','F','G','H','J','K','L',':','"','~', // 0x1E-0x29
    LSHIFT,     // 0x2A
    '|',        // 0x2B
    'Z','X','C','V','B','N','M','<','>','?', // 0x2C-0x35
    RSHIFT,     // 0x36
    '*',        // 0x37
    LALT,       // 0x38
    ' ',        // 0x39
    CAPSLOCK,   // 0x3A
    FUNCTION1, FUNCTION2, FUNCTION3, FUNCTION4, FUNCTION5,
    FUNCTION6, FUNCTION7, FUNCTION8, FUNCTION9, FUNCTION10,
    NUMLOCK,    // 0x45
    0,          // 0x46
    '7','8','9',
    '-',
    '4','5','6',
    '+',
    '1','2','3',
    '0',
    '.',
    0,0,
    0
};

static const unsigned char scancode_map_altgr_02[] = {
    0, // US layout has no AltGr mappings, all zeros
    0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,'*',0,' ',0,
    FUNCTION1,FUNCTION2,FUNCTION3,FUNCTION4,FUNCTION5,
    FUNCTION6,FUNCTION7,FUNCTION8,FUNCTION9,FUNCTION10,
    0,0,
    '7','8','9','-','4','5','6','+','1','2','3','0','.',
    0,0,0
};

// 0x03 - English UK
static const unsigned char scancode_map_03[] = {
    0,          // 0x00
    ESC,        // 0x01
    '1','2','3','4','5','6','7','8','9','0','-','=', // 0x02-0x0D
    '\b',       // 0x0E
    '\t',       // 0x0F
    'q','w','e','r','t','y','u','i','o','p','[',']', // 0x10-0x1B
    '\n',       // 0x1C
    LCTRL,      // 0x1D
    'a','s','d','f','g','h','j','k','l',';','\'','`', // 0x1E-0x29
    LSHIFT,     // 0x2A
    '#',        // 0x2B (UK has # here, not \)
    'z','x','c','v','b','n','m',',','.','/', // 0x2C-0x35
    RSHIFT,     // 0x36
    '*',        // 0x37
    LALT,       // 0x38
    ' ',        // 0x39
    CAPSLOCK,   // 0x3A
    FUNCTION1, FUNCTION2, FUNCTION3, FUNCTION4, FUNCTION5,
    FUNCTION6, FUNCTION7, FUNCTION8, FUNCTION9, FUNCTION10,
    NUMLOCK,    // 0x45
    0,          // 0x46
    '7','8','9',
    '-',
    '4','5','6',
    '+',
    '1','2','3',
    '0',
    '.',
    0,0,
    '\\'        // 0x56 UK has \ here
};

static const unsigned char scancode_map_shift_03[] = {
    0,          // 0x00
    ESC,        // 0x01
    '!','"',0xA3,'$','%','^','&','*','(',')','_','+', // 0x02-0x0D (0xA3=£ approximation)
    '\b',       // 0x0E
    '\t',       // 0x0F
    'Q','W','E','R','T','Y','U','I','O','P','{','}', // 0x10-0x1B
    '\n',       // 0x1C
    LCTRL,      // 0x1D
    'A','S','D','F','G','H','J','K','L',':','@','~', // 0x1E-0x29 (UK: @ on shift+')
    LSHIFT,     // 0x2A
    '~',        // 0x2B
    'Z','X','C','V','B','N','M','<','>','?', // 0x2C-0x35
    RSHIFT,     // 0x36
    '*',        // 0x37
    LALT,       // 0x38
    ' ',        // 0x39
    CAPSLOCK,   // 0x3A
    FUNCTION1, FUNCTION2, FUNCTION3, FUNCTION4, FUNCTION5,
    FUNCTION6, FUNCTION7, FUNCTION8, FUNCTION9, FUNCTION10,
    NUMLOCK,    // 0x45
    0,          // 0x46
    '7','8','9',
    '-',
    '4','5','6',
    '+',
    '1','2','3',
    '0',
    '.',
    0,0,
    '|'         // 0x56 shifted
};

static const unsigned char scancode_map_altgr_03[] = {
    0,          // 0x00
    ESC,        // 0x01
    0,0,0,'$',0,0,0,0,0,0,0,0, // 0x02-0x0D (AltGr+4 = € approximated as $)
    '\b',       // 0x0E
    '\t',       // 0x0F
    0,0,0,0,0,0,0,0,0,0,0,0, // 0x10-0x1B
    '\n',       // 0x1C
    LCTRL,      // 0x1D
    0,0,0,0,0,0,0,0,0,0,0,0, // 0x1E-0x29
    LSHIFT,     // 0x2A
    0,          // 0x2B
    0,0,0,0,0,0,0,0,0,0, // 0x2C-0x35
    RSHIFT,     // 0x36
    '*',        // 0x37
    LALT,       // 0x38
    ' ',        // 0x39
    CAPSLOCK,   // 0x3A
    FUNCTION1, FUNCTION2, FUNCTION3, FUNCTION4, FUNCTION5,
    FUNCTION6, FUNCTION7, FUNCTION8, FUNCTION9, FUNCTION10,
    NUMLOCK,    // 0x45
    0,          // 0x46
    '7','8','9',
    '-',
    '4','5','6',
    '+',
    '1','2','3',
    '0',
    '.',
    0,0,
    0
};
void set_ScancodeMapID(uint8_t map) {
    uint8_t prev = scancodeid;
    scancodeid = map;
    if (map != 0 && get_Scancode() == scancode_map_00) scancodeid = prev;
}
void set_ScancodeMapID(int map) {
    if (map == 0) map = 0x00;
    else if (map == 1) map = 0x01;
    else if (map == 2) map = 0x02;
    else if (map == 3) map = 0x03;
    else return;
    scancodeid = map;
}
uint8_t get_ScancodeMapSize() {
    if (scancodeid == 0x00) return sizeof(scancode_map_00);
    if (scancodeid == 0x01) return sizeof(scancode_map_01);
    if (scancodeid == 0x02) return sizeof(scancode_map_02);
    if (scancodeid == 0x03) return sizeof(scancode_map_03);
    return sizeof(scancode_map_00);
}
const unsigned char* get_Scancode() {
    if (scancodeid == 0x00) return scancode_map_00;
    if (scancodeid == 0x01) return scancode_map_01;
    if (scancodeid == 0x02) return scancode_map_02;
    if (scancodeid == 0x03) return scancode_map_03;
    return scancode_map_00; // fallback to es_es
}
const unsigned char* get_Scancode_Shifted() {
    if (scancodeid == 0x00) return scancode_map_shift_00;
    if (scancodeid == 0x01) return scancode_map_shift_01;
    if (scancodeid == 0x02) return scancode_map_shift_02;
    if (scancodeid == 0x03) return scancode_map_shift_03;
    return scancode_map_shift_00;
}
const unsigned char* get_Scancode_Altgr() {
    if (scancodeid == 0x00) return scancode_map_altgr_00;
    if (scancodeid == 0x01) return scancode_map_altgr_01;
    if (scancodeid == 0x02) return scancode_map_altgr_02;
    if (scancodeid == 0x03) return scancode_map_altgr_03;
    return scancode_map_altgr_00;
}