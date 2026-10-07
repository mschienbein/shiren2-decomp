#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

extern u32 D_8013960C;
s32 func_80049CB4(s32, ...);
void func_800EDF78(u8 *, s32);
char *func_80048480(u16);
void func_800498E4(s32, ...);
s32 func_800A533C(u8 *);
void func_800EDE50(u8 *self, s32 silent) {
    s32 msg;

    if (!silent) {
        func_80049CB4(0x71, self);
        func_80049CB4(0x88, self);
    }
    D_8013960C <<= 1;
    func_800EDF78(self, silent ^ 1);
    D_8013960C |= 1;
    msg = 0x52;
    if (self[0x109] == 0x15) {
        msg = 0x53;
    }
    func_800498E4(msg, func_80048480(0x8DF));
    D_8013960C >>= 1;
    if (!silent) {
        func_80049CB4(0x1F, self);
        func_800A533C(self);
    }
    self[0x109] = 0;
    *(u16 *)(self + 0xE4) &= ~0x40;
    func_80049CB4(0x89, self);
    func_80049CB4(2);
}
