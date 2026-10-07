#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { u8 pad0[0x50]; u16 value; u8 pad52[4]; u8 count; } Obj;
extern s16 D_80158C6C[];
s32 func_800E04D0(Obj *o);
void func_800E4E24(Obj *o, s32 amount) {
    s32 v;
    s32 step;
    o->count = 0;
    v = o->value - amount;
    while ((s16)(o->value = v) <= 0) {
        o->count++;
        step = D_80158C6C[(u8)func_800E04D0(o)];
        v = o->value + step;
    }
}
