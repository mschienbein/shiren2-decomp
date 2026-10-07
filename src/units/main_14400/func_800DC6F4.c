#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { s32 field_0; void *vtable_4; } Obj800DC6F4;
extern u8 D_80157FA8[];
void func_800D8FE8(Obj800DC6F4 *obj);

void func_800DC6F4(Obj800DC6F4 *obj, s32 flags) {
    obj->vtable_4 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
