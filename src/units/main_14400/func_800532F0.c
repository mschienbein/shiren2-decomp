#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Pair;

extern u8 D_801398A0;
extern Pair D_80161700;
extern Pair D_80161708;
extern s32 D_80161710;

extern void func_8012A2F8(s32 a, s32 b);

void func_800532F0(u8 scale) {
    if (scale == 0) {
        func_8012A2F8(3, 1);
    } else {
        D_801398A0 = 2;
        D_80161700.x = 0x4FFF;
        D_80161700.y = 0x4FFF / scale;
        D_80161708.x = 0x67FF;
        D_80161708.y = -(0x67FF / scale);
        D_80161710 = scale;
        D_80161700.y = -D_80161700.y;
    }
}
