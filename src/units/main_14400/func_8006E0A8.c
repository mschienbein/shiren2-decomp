#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad0[2]; u16 buttons; u8 pad4[0xE]; u16 buttons2; } Input8006E0A8;

s32 func_8006E0A8(Input8006E0A8 *input) {
    u16 held;
    u16 held2;
    s32 result;

    held = input->buttons;
    result = -1;
    if (held & 0x800F) {
        result = 0x33;
    } else if (held & 0x4000) {
        result = 0x34;
    } else if (held & 0x10) {
        result = 0x35;
    } else if (held & 0x20) {
        result = 0x36;
    } else if (held & 0x1000) {
        result = 0x37;
    } else {
        held2 = input->buttons2;
        if (held2 & 0x800) {
            result = 0x38;
        } else if (held2 & 0x400) {
            result = 0x39;
        } else if (held2 & 0x200) {
            result = 0x3A;
        } else if (held2 & 0x100) {
            result = 0x3B;
        }
    }
    return result;
}
