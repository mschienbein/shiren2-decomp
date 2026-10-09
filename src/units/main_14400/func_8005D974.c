#include "common.h"
typedef unsigned char u8;
/* Whole 0x20-byte controller cleared by func_8005DA84. */
typedef struct {
    short x0, y0, x1, y1;
    unsigned short x2, y2;
    u8 phase, mode;
    u8 level_c, target_c, level_b, target_b, level_a, target_a;
    u8 flags, reserved15, period, tick;
    u8 first, second, selection, reserved1B[5];
} RampState;
extern RampState D_80165960;
s32 func_8005CC34(s32, s32, s32, s32, s32, s32);
void func_8005DB6C(void);
void func_8005D974(s32 x, s32 y, s32 x0, s32 y0, s32 first, s32 second){
    RampState *s = &D_80165960;
    if (first != -1) {
        func_8005CC34(0, 0x127, x, y, 0, first);
        if (first != second) func_8005CC34(1, 0x127, x, y, 1, second);
    }
    s->x0 = x0;
    s->y0 = y0;
    s->x1 = x;
    s->y1 = y;
    s->x2 = x;
    s->y2 = y;
    s->phase = 0;
    s->level_c = 0;
    s->target_c = 0;
    s->level_b = 0;
    s->target_b = 0;
    s->level_a = 0;
    s->target_a = 0;
    s->period = 0;
    s->tick = 0;
    s->first = first;
    s->second = second;
    s->selection = 0xFF;
    s->flags |= 1;
    func_8005DB6C();
}
