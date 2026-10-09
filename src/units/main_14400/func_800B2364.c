#include "common.h"

extern unsigned char D_80143391;
extern s32 func_80049CB4(s32 id, ...);

void func_800B2364(s32 enable) {
    if (enable) {
        D_80143391 |= 1;
    } else {
        D_80143391 &= ~1;
    }
    func_80049CB4(0xDB);
}
