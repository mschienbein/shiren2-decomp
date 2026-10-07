#include "common.h"

typedef unsigned short u16;

typedef struct {
    u16 field_0;
    u16 buttons;
} Input8006DB20;

/* D_8013D3F0 handler: func_8006D550 passes the current input record (a0) and its saved
 * input record (a1, 0x8006D848); this handler leaves `saved` unused. */
s32 func_8006DB20(Input8006DB20 *input, void *saved)
{
    s32 result = -1;
    u16 buttons = input->buttons;

    if (buttons & 0x1000) {
        result = 0;
    } else if (buttons & 0x8000) {
        result = 1;
    } else if (buttons & 0x4000) {
        result = 2;
    } else if (buttons & 0x200) {
        result = 0x15;
    } else if (buttons & 0x100) {
        result = 0x16;
    } else if (buttons & 0x800) {
        result = 0x13;
    } else if (buttons & 0x400) {
        result = 0x14;
    } else if (buttons != 0) {
        result = 3;
    }
    return result;
}
