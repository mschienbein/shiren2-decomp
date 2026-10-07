#include "common.h"

typedef unsigned char u8;

typedef struct { s32 field_0; void *handler; } Obj800DC270;

extern u8 D_80157FA8[];
void func_800D8FE8(Obj800DC270 *obj);

void func_800DC270(Obj800DC270 *obj, s32 flags) {
    obj->handler = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
