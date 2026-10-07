#include "common.h"

typedef unsigned short u16;

typedef struct {
    u16 field_0;
    u16 held;
    unsigned char pad4[0xE];
    u16 pressed;
} Input8006E14C;

/* D_8013D3F0 handler: func_8006D550 passes the current input record (a0) and its saved
 * input record (a1, 0x8006D848); this handler leaves `saved` unused. */
s32 func_8006E14C(Input8006E14C *input, void *saved)
{
    u16 held = input->held;
    s32 result = -1;

    if (held & 0x8000) {
        result = 0x33;
    } else if (held & 0x4000) {
        result = 0x34;
    } else if (held & 0xF) {
        result = 0x35;
    } else if (held & 0x1000) {
        result = 0x37;
    } else {
        u16 pressed = input->pressed;

        if (pressed & 0x800) {
            result = 0x38;
        } else if (pressed & 0x400) {
            result = 0x39;
        } else if (pressed & 0x200) {
            result = 0x3A;
        } else if (pressed & 0x100) {
            result = 0x3B;
        }
    }
    return result;
}
