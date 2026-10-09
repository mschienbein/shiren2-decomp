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

void func_8005DB6C(void)
{
    RampState *state = &D_80165960;

    if (state->flags == 0) {
        return;
    }
    if (state->period != 0) {
        if (++state->tick >= state->period) {
            state->tick = 0;
        }
    }
    if (state->flags & 0x10) {
        return;
    }
    if (state->target_a != 0) {
        if (state->flags & 2) {
            if (state->level_a < state->target_a) {
                state->level_a++;
            }
        } else {
            if (state->level_a == 0) {
                state->flags = 0;
                return;
            }
            state->level_a--;
        }
    }
    if (state->target_c != 0) {
        if (!(state->flags & 4)) {
            if (state->level_c < state->target_c) {
                state->level_c++;
            }
        } else if (state->level_c != 0) {
            state->level_c--;
        }
    }
    if (state->target_b != 0) {
        if (!(state->flags & 8)) {
            if (state->level_b < state->target_b) {
                state->level_b++;
            }
        } else if (state->level_b != 0) {
            state->level_b--;
        }
    }
}
