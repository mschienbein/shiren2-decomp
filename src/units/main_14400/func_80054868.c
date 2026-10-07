#include "common.h"

typedef unsigned char u8;

extern u8 D_80161B44;
void func_80053D70(s32, s32, s32, s32, s32, s32, char *, void *);
void func_80054868(char *fmt, void *args) {
    D_80161B44 = 2;
    func_80053D70(0, 0, 5, 0, 0x1E, 0x1D, fmt, args);
}
