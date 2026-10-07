#include "common.h"

/* One initialized word at ROM 0x113190 / VRAM 0x80140150. Both original
 * accessors use this word in their return delay slots. */
s32 D_80140150 = 1;

void func_800925E0(s32 value) {
    D_80140150 = value;
}

s32 func_800925EC(void) {
    return D_80140150;
}
