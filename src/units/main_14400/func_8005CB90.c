#include "common.h"

typedef unsigned char u8;

extern u8 D_801DE984[];
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
s32 func_8006E908(void *pool, s32 count1, s32 count2, s32 count3);

void func_8005CB90(void) {
    func_8006E908(D_801DE984, 0, 0, 2);
    D_80165960.flags = 0;
}
