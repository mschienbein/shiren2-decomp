#include "common.h"

typedef unsigned char u8;

extern u8 D_80157FA8[];
typedef struct { s32 x0; void *x4; } Obj;
void func_800D8FE8(Obj *obj);
void func_800DF70C(Obj *obj, s32 flags) {
    obj->x4 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
