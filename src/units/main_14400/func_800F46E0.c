#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 pad0[0xA]; u8 field_A; } Obj800F46E0;
s32 func_800E0F40(Obj800F46E0 *obj);
u16 func_800F4044(s32 id, u8 b);
u16 func_800F46E0(Obj800F46E0 *obj) {
    s32 id = obj->field_A;
    return func_800F4044(id, func_800E0F40(obj));
}
