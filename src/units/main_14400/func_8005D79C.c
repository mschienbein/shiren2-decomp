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
typedef struct { float x0; float y0; float x1; float y1; } Rect;
extern RampState D_80165960;
extern void func_8005D0DC(Rect *);

void func_8005D79C(void) {
    Rect rectangle;
    Rect *rect = &rectangle;
    RampState *state = &D_80165960;
    if (state->flags && state->level_a == state->target_a && !(state->flags & 0x20)) {
        func_8005D0DC(rect);
        if (rect->x0 >= rect->x1 || rect->y0 >= rect->y1) {
            return;
        }
        if (rect->x0 < 12.0f) rect->x0 = 12.0f;
        if (rect->y0 < 10.0f) rect->y0 = 10.0f;
        if (rect->x1 > 308.0f) rect->x1 = 308.0f;
        if (rect->y1 > 230.0f) rect->y1 = 230.0f;
    }
}
