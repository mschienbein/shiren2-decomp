#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
extern void *D_801476B8;
s32 func_800A3B10(u8);
s32 func_800A3A20(u8);
s32 func_800A3A7C(u32 arg0);
s32 func_800A3AF0(u8);
s32 func_800A3B00(u8);
s32 func_800A3AE0(u8);
s32 func_800F47F0(u8);
char *func_800A3B20(void *);
s32 func_800EE58C(u8);
u16 func_800EFDAC(u8 kind, u8 variant);
s32 func_800F3FEC(u8, u8);
char *func_80048480(u16);
char *func_800A8498(s32 id, s32 arg) {
    u16 text = 0;
    u8 index = id;
    if (func_800A3B10(index)) {
        text = func_800F47F0(index);
    } else if (func_800A3A20(index)) {
        return func_800A3B20(D_801476B8);
    } else if (func_800A3A7C(index)) {
        text = func_800EE58C(index);
    } else if (func_800A3AF0(index)) {
        text = func_800EFDAC(index, arg);
    } else if (func_800A3B00(index)) {
        text = func_800F3FEC(index, arg);
    } else if (func_800A3AE0(index)) {
        text = (u8)id + 0x26B2;
    }
    return func_80048480(text);
}
