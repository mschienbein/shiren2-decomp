#include "common.h"

typedef unsigned char u8;
typedef struct { char pad0[0x75]; u8 unk75; } Obj;
void func_800512BC(s32, Obj *, s32, s32, s32);
void func_80051208(s32 arg0, Obj *obj, s32 flags) {
    s32 v;
    if (flags & 0x10000) {
        v = 0;
    } else if (flags & 0x20000) {
        v = 1;
    } else if (flags & 0x40000) {
        v = 3;
    } else {
        v = obj->unk75 - 1;
    }
    func_800512BC(arg0, obj, flags, v, 1);
}
