#include "common.h"

typedef unsigned char u8;

extern u8 D_80161B44;
extern s32 D_8016172C;
void func_80053D70(s32, s32, s32, s32, s32, s32, char *, void *);
void func_800547E0(s32 mode, char *fmt, void *args) {
    D_80161B44 = 1;
    func_80053D70(0, mode, 5, 0x15, 0x1E, 6, fmt, args);
    D_8016172C = 2;
}
