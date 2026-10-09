#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

extern s32 D_80142D24;
extern u8 D_801536CC[];
extern u8 D_801536D0[];
extern u8 D_801536D4[];
u8 D_801C51FA[3];
void func_800D8F10(s32 code) __attribute__((noreturn));
void func_800A9C8C(u8 id, u8 value);

void func_800A9E20(u8 id) {
    if (D_80142D24 == 0) {
        D_80142D24 = 1;
        D_801C51FA[0] = D_801536CC[0] + 1;
        D_801C51FA[1] = D_801536D0[0] + 1;
        D_801C51FA[2] = D_801536D4[0] + 1;
    }
    if ((u8)(id - 1) >= 3) {
        func_800D8F10(1);
    }
    func_800A9C8C(id, D_801C51FA[id - 1]);
}
