#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    s32 x;
    s32 y;
} Pos800C532C;

typedef struct {
    s32 x_0;
    s32 y_4;
    u16 speed_8;
    u16 maxSpeed_A;
    s16 offsetY_C;
    s16 offsetX_E;
    u8 dir_10;
    u8 pad11;
    u16 timer_12;
} Wobble800C532C;

extern s8 D_801475F0[][2];
extern unsigned char D_80147620[];
s32 func_800C5A1C(void *rng, s32 base, s32 top);
static inline void offset_position(Pos800C532C *pos, Wobble800C532C *w, s32 dx, s32 dy) {
    s32 x = w->x_0;
    s32 y = w->y_4;
    s32 next_x = x + dx;
    s32 next_y = y + dy;
    pos->x = next_x;
    pos->y = next_y;
}

Pos800C532C *func_800C532C(Pos800C532C *out, Wobble800C532C *w) {
    Pos800C532C pos;
    s16 dx;
    s16 dy;

    offset_position(&pos, w, w->offsetX_E, w->offsetY_C);
    if (w->speed_8 <= w->maxSpeed_A) {
        if (++w->timer_12 >= w->speed_8 * 8) {
            w->speed_8++;
            w->timer_12 = 0;
            w->dir_10 = func_800C5A1C(&D_80147620, 0, 3);
            if (w->dir_10 == 0 || w->dir_10 == 2) {
                w->offsetY_C = func_800C5A1C(&D_80147620, -w->speed_8, w->speed_8);
                w->offsetX_E = (w->dir_10 == 0) ? -w->speed_8 : w->speed_8;
            } else {
                w->offsetY_C = (w->dir_10 == 1) ? w->speed_8 : -w->speed_8;
                w->offsetX_E = func_800C5A1C(&D_80147620, -w->speed_8, w->speed_8);
            }
        }
        w->offsetY_C += D_801475F0[w->dir_10][0];
        w->offsetX_E += D_801475F0[w->dir_10][1];
        dy = (w->offsetY_C < 0) ? -w->offsetY_C : w->offsetY_C;
        dx = (w->offsetX_E < 0) ? -w->offsetX_E : w->offsetX_E;
        if (w->speed_8 < dy || w->speed_8 < dx) {
            w->offsetY_C -= D_801475F0[w->dir_10][0];
            w->offsetX_E -= D_801475F0[w->dir_10][1];
            w->dir_10 = (w->dir_10 < 3) ? w->dir_10 + 1 : 0;
            w->offsetY_C += D_801475F0[w->dir_10][0];
            w->offsetX_E += D_801475F0[w->dir_10][1];
        }
    }
    out->x = pos.x;
    out->y = pos.y;
    return out;
}
