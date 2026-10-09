#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x84];
    u8 scaleA;
    u8 scaleB;
    u8 max;
    u8 pad87[0x9D - 0x87];
    u8 level;
} Obj800F04EC;

extern u32 D_8013960C;
s32 func_800E07A8(Obj800F04EC *obj, s32 amount);
s32 func_800E0534(Obj800F04EC *obj, s32 amount);
/* Signed-word amount and explicit signed-halfword result (0x800E0BE8, 0x800E0C20); the
 * short amountB keeps its own narrowing before the call (0x800F05A8). */
short func_800E0BD0(Obj800F04EC *obj, s32 amount);
s32 func_800E0AB4(Obj800F04EC *obj, s32 amount);

void func_800F04EC(Obj800F04EC *obj, s16 delta) {
    u8 level = obj->level;
    s16 sum = level + delta;
    u8 value = sum;
    s16 amountA;
    s16 amountB;

    if (sum < 0) {
        value = 0;
        delta = -level;
    } else if (sum > obj->max) {
        value = obj->max;
        delta = value - level;
    }
    amountA = delta * obj->scaleA;
    amountB = delta * obj->scaleB;
    obj->level = value;
    D_8013960C *= 2;
    func_800E07A8(obj, amountA);
    func_800E0534(obj, amountA);
    func_800E0BD0(obj, amountB);
    func_800E0AB4(obj, amountB);
    D_8013960C /= 2;
}
