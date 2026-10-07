#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { s32 field_0; void *vtable; } Obj800DBFB8;
extern u8 D_80157FA8[];
void func_800D8FE8(void *obj);
void func_800DBFB8(Obj800DBFB8 *obj, s32 flags) {
    obj->vtable = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
