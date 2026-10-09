#include "common.h"
typedef struct { s32 field_0; void *field_4; } Obj;
extern unsigned char D_80157FA8[];
extern void func_800D8FE8(void *object);
void func_800DA12C(Obj *obj, s32 flags) {
    obj->field_4 = D_80157FA8;
    if (flags & 1) func_800D8FE8(obj);
}
