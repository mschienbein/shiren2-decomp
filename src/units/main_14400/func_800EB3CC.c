#include "common.h"

typedef struct {
    unsigned char pad0[0x88];
    s32 value;
    s32 max;
} Obj;

s32 func_800EB37C(Obj *obj);
s32 func_800EB3A4(Obj *obj);
void func_800498E4(s32 id, ...);

void func_800EB3CC(Obj *obj, short amount)
{
    s32 value;

    if (amount > 0 && obj->value < 0) {
        obj->value = 100;
    }
    value = amount * 1000;
    value += obj->value;
    if (value > obj->max) {
        value = obj->max;
    } else if (value < -300) {
        value = -300;
    }
    obj->value = value;
    if (amount > 0) {
        s32 first = func_800EB37C(obj) & 0xFF;

        func_800498E4(first == (func_800EB3A4(obj) & 0xFF) ? 0x14 : 0x13);
    }
}
