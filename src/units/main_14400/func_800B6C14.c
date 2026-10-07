#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x10]; u8 counts[4]; } Obj800B6C14;
typedef struct { s32 value; } Result800B6C14;
extern u8 D_80147620[];
s32 func_800B68B0(Obj800B6C14 *obj);
u16 func_800C58DC(void *rng, u16 range);
void func_800B6BA0(Result800B6C14 *out, Obj800B6C14 *obj);
void *func_800B6A98(void *out, void *obj, s32 value);
Result800B6C14 *func_800B6C14(Result800B6C14 *out, Obj800B6C14 *obj, s32 index) {
    s32 value;
    s32 sum;
    s32 i;

    value = func_800B68B0(obj);
    if (value == 0 || value == obj->counts[index]) {
        func_800B6BA0(out, obj);
        return out;
    }
    value = func_800C58DC(D_80147620, value - obj->counts[index] - 1);
    sum = 0;
    for (i = 0; i < index; i++) {
        sum += obj->counts[i];
    }
    if (value >= sum) {
        value += obj->counts[index];
    }
    func_800B6A98(out, obj, value);
    return out;
}
