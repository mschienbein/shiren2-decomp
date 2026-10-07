#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x20]; s32 x20; u8 pad24[0x10]; s32 x34; s32 x38; } Obj;
s32 func_80098E34(Obj *obj, s32 offset);
s32 func_80099FFC(Obj *obj, s32 index);
s32 func_80099E58(Obj *obj, s32 flag);
s32 func_80099F98(Obj *obj) {
    s32 index = func_80098E34(obj, obj->x34 + obj->x20 * obj->x38);
    if (index < 0) {
        return 0;
    }
    return func_80099E58(obj, func_80099FFC(obj, index) ^ 1);
}
