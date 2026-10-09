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

void func_8005D88C(u8 target_c, u8 target_b, u8 snap_c, u8 snap_b)
{
    RampState *state = &D_80165960;

    state->target_c = target_c;
    state->target_b = target_b;
    if (snap_c) {
        state->level_c = target_c;
        state->flags |= 4;
    } else {
        state->level_c = 0;
        state->flags &= ~4;
    }
    if (snap_b) {
        state->level_b = target_b;
        state->flags |= 8;
    } else {
        state->level_b = 0;
        state->flags &= ~8;
    }
}
