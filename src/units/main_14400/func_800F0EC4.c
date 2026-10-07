#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 pad0[0x90]; s16 offset_90; s16 pad92; s32 (*fn_94)(void *self, s32 a, s32 b, u8 c, s32 d); } VTable800F0EC4;
typedef struct { u8 pad0[0x24]; VTable800F0EC4 *vtable_24; } Obj800F0EC4;
s32 func_800E1CC4(Obj800F0EC4 *obj, s32 kind);
s32 func_800E1D14(Obj800F0EC4 *obj, s32 kind);
u16 func_800E08B0(Obj800F0EC4 *obj);

s32 func_800F0EC4(Obj800F0EC4 *obj) {
    s32 result = 0;

    if (func_800E1CC4(obj, 2) || func_800E1D14(obj, 0x13) || func_800E1CC4(obj, 1) ||
        func_800E1CC4(obj, 0) ||
        obj->vtable_24->fn_94((u8 *)obj + obj->vtable_24->offset_90, 2, 9, 0, 0) ||
        func_800E08B0(obj) == 0) {
        result = 1;
    }
    return result;
}
