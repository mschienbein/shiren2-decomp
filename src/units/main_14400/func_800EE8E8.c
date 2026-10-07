#include "common.h"

typedef unsigned char u8;
typedef struct { char pad0[0x58]; void *unk58; } Obj;
s32 func_800EA38C(Obj *, s32, s32, u8, s32);
s32 func_800A44F4(Obj *, void *);
s32 func_800EE8E8(Obj *obj, s32 mode, s32 arg2, u8 arg3, s32 arg4) {
    s32 ret = func_800EA38C(obj, mode, arg2, arg3, arg4);
    if (mode == 1) {
        if (func_800A44F4(obj, obj->unk58) != 2) {
            obj->unk58 = 0;
        }
    }
    return ret;
}
