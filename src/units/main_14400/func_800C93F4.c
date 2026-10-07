#include "common.h"

typedef unsigned short u16;
extern u16 D_8014767C;
s32 func_80049CB4(s32 id, ...);
void func_800C93F4(void) {
    D_8014767C = (D_8014767C & ~0xC) | 4;
    func_80049CB4(0x126, 0x20);
}
