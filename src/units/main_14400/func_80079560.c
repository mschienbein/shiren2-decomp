#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { u8 pad[2]; s16 x2; u8 pad4[4]; u8 x8; u8 pad9; u16 xA; u8 padC[0xC]; s32 x18; } S;
extern S *func_8007946C(s32, s32);
void func_80079560(s32 a, s32 b, s32 mode){
    S *p = func_8007946C(a, b);
    if (p == 0) return;
    if (p->x2 == -1) return;
    if (p->x8 == mode) return;
    if (mode == 1) {
        if (p->xA & 0x10) p->x18 = 30;
    }
    p->x8 = mode;
}
