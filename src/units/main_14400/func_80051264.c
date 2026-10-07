#include "common.h"

typedef unsigned char u8;
typedef struct { char pad[0x75]; u8 count75; } Obj;
void func_800512BC(s32, Obj *, s32, s32, s32);
void func_80051264(s32 arg0, Obj *obj, s32 flags) {
    s32 index;
    if (flags & 0x10000) {
        index = 0;
    } else if (flags & 0x20000) {
        index = 1;
    } else if (flags & 0x40000) {
        index = 3;
    } else {
        index = obj->count75 - 1;
    }
    func_800512BC(arg0, obj, flags, index, 0);
}
