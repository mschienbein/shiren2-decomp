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
extern void func_8005CE44(s32);
void func_8005DB20(void) { unsigned char *p = &D_80165960.flags; if (*p) { *p = 0; func_8005CE44(0); func_8005CE44(1); } }
