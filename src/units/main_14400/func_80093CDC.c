#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x, y; } Pos;
typedef struct { s32 x0; u8 pad4[0xC]; u8 x10; } Mover;
typedef struct { Mover *x0; s8 x4; u8 pad5[0x87]; s32 x8C; } Stage;
/* Player unit (D_801476B8): map position at +0, partner unit at +0x104 (see
 * func_800C5F60 and func_800EBCD0). */
typedef struct Unit {
    Pos pos;
    u8 pad8[0x104 - 0x8];
    struct Unit *partner_104;
} Unit;
extern s32 D_801541E4[];
extern s32 D_80151A7C[];
extern s32 D_80151A88[];
const u16 D_80151A94[3] = { 0x4A4, 0x4A5, 0x4A6 };
extern u16 D_801476BE;
extern Unit *D_801476B8;
extern u32 D_8013960C;
s32 func_80046240(void);
s32 func_80049CB4(s32 id, ...);
void func_800EBCD0(Unit *unit);
Unit *func_800C5F60(void);
void func_800498E4(s32 id, ...);
s32 func_80093CDC(Stage *st) {
    Pos tmp;
    if (st->x4 == 0 && !func_80046240()) {
        do {
            s32 ready = D_801541E4[st->x0->x10] - st->x0->x0 < D_80151A7C[st->x8C]
                     || D_80151A88[st->x8C] < D_801476BE;
            if (!ready) {
                return 0;
            }
            func_80049CB4(9);
            if (st->x8C + 1 >= 3 && D_801476B8->partner_104 != 0) {
                D_8013960C *= 2;
                func_800EBCD0(D_801476B8);
                D_8013960C /= 2;
                func_80049CB4(0x93, func_800C5F60());
                func_80049CB4(0xDD);
            }
            func_800498E4(D_80151A94[st->x8C]);
            func_80049CB4(0x92, func_800C5F60(), st->x8C + 1);
            {
                Pos *dst = &tmp;
                Pos *src = &func_800C5F60()->pos;
                dst->x = src->x;
                dst->y = src->y;
            }
            func_80049CB4(0xF7, &tmp, st->x8C + 1);
            func_80049CB4(2);
        } while (++st->x8C < 3);
        return 1;
    }
    return 0;
}
