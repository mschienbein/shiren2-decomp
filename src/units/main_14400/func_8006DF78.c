#include "common.h"
typedef unsigned short u16;
extern s32 D_8013D418[];
extern void func_800740A8(s32);
static inline s32 direction_index(s32 code) { return code - 0x13; }
s32 func_8006DF78(u16 buttons, u16 previous, s32 mode) {
    s32 result = -1;
    if ((buttons & 0xFFEF) != previous) {
        if ((buttons & 0xA00) == 0xA00) result = 0x17;
        else if ((buttons & 0x900) == 0x900) result = 0x18;
        else if ((buttons & 0x600) == 0x600) result = 0x19;
        else if ((buttons & 0x500) == 0x500) result = 0x1A;
        else if (!(buttons & 0x10)) {
            if (buttons & 0x800) result = 0x13;
            else if (buttons & 0x400) result = 0x14;
            else if (buttons & 0x200) result = 0x15;
            else if (buttons & 0x100) result = 0x16;
        }
    }
    if (result != -1) {
        result = direction_index(result) + D_8013D418[mode];
        if (mode == 3) func_800740A8(1);
    }
    return result;
}
