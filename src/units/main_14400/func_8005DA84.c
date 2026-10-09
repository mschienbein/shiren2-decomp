#include "common.h"
typedef unsigned char u8;
/* Whole 0x20-byte controller: the clear below establishes its full extent. */
typedef struct {
    short x0, y0, x1, y1;
    unsigned short x2, y2;
    u8 phase, mode;
    u8 level_c, target_c, level_b, target_b, level_a, target_a;
    u8 flags, reserved15, period, tick;
    u8 first, second, selection, reserved1B[5];
} RampState;
extern RampState D_80165960;
extern void func_800265E0(void *, s32);
extern s32 func_8005CD40(s32, s32, s32, s32, s32, s32); /* status result intentionally discarded */
extern void func_8005DB6C(void);
void func_8005DA84(void) { RampState *object = &D_80165960; func_800265E0(object, 0x20); func_8005CD40(0, 0x188, 0xFA, 0xBE, 0, 0); object->x0 = 0x18; object->y0 = 0x18; object->x1 = 0xFA; object->y1 = 0xBE; object->x2 = 0xFA; object->y2 = 0xBE; object->flags = 0x21; object->first = 0; object->second = 0; object->selection = 0xFF; func_8005DB6C(); }
