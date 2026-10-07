#include "common.h"

typedef unsigned short u16;
typedef struct { s32 x0; u16 x4; u16 x6; u16 state8; u16 xA; u16 xC; u16 xE; u16 x10; u16 flags12; s32 x14; s32 x18; s32 timer1C; } S;
void func_8005A670(s32 a, s32 b, s32 c);
s32 func_80076044(s32 slot, s32 animation, s32 mode, s32 frame, s32 flags);
void func_80087B2C(S *p) {
    s32 state = p->state8;
    switch (state) {
    case 0:
        if (p->flags12 & 1) func_8005A670(0, 0, 0);
        p->state8++;
        break;
    case 1:
        func_80076044(p->x14, 0xA9, 2, 0x20, 0);
        p->xE = state;
        p->timer1C = 0x28;
        p->state8++;
        break;
    case 2:
        if (p->timer1C-- == 0) {
            func_80076044(p->x14, 0xA9, 1, 8, 3);
            p->x4 = 4;
        }
        break;
    }
}
