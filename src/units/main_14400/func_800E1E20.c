#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef struct { u8 pad[0x90]; s16 delta; s16 idx; s32 (*fn)(void *, s32, s32, u8, s32); } VTable;
typedef struct { u8 pad0[0x24]; VTable *vt; } Obj;
s32 func_800E1DD8(Obj *o);
s32 func_800E1CC4(Obj *o, s32 kind);
s32 func_800E1E08(Obj *o);
s32 func_800E1E14(Obj *o);
u32 func_800E110C(const Obj *o);
s32 func_800E1E20(Obj *o) {
    s32 result = 0;
    if (func_800E1DD8(o) && !func_800E1CC4(o, 3)) {
        result = 1;
    } else if (func_800E1E08(o)) {
        result = 1;
    } else if (func_800E1E14(o)) {
        result = 1;
    } else if (o->vt->fn((u8 *)o + o->vt->delta, 2, 0x11, 0, 0) && !(s8)func_800E110C(o)) {
        result = 1;
    }
    return result;
}
