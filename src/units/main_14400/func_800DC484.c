#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 field_0; void *vtable; } Obj;
extern u8 D_80157FA8[];
void func_800D8FE8(Obj *obj);
void func_800DC484(Obj *obj, s32 flags) {
    obj->vtable = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
