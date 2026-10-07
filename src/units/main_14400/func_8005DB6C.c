#include "common.h"

typedef unsigned char u8;

/* Fade/ramp controller: three counters ramp toward their targets per flag bit. */
typedef struct {
    u8 pad0[0xE];
    u8 level_c;     /* 0x0E */
    u8 target_c;    /* 0x0F */
    u8 level_b;     /* 0x10 */
    u8 target_b;    /* 0x11 */
    u8 level_a;     /* 0x12 */
    u8 target_a;    /* 0x13 */
    u8 flags;       /* 0x14 */
    u8 pad15;
    u8 period;      /* 0x16 */
    u8 tick;        /* 0x17 */
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
