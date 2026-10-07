#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

extern u16 D_80156A10;
u32 func_800B1C6C(void *);
u16 func_800E08F0(u8 *);
void func_800A7B18(void *, void *, s32, s32);
s32 func_800E0970(u8 *self) {
    u16 amount;

    if (!(func_800B1C6C(self) & 0x4000)) {
        return 0;
    }
    amount = func_800E08F0(self) * D_80156A10 / 100;
    if (amount == 0) {
        amount = 1;
    }
    func_800A7B18(self, 0, amount, 0x24);
    return 1;
}
