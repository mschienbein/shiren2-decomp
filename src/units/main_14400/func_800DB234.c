#include "common.h"
typedef struct { s32 field_00; void *vtable_04; } Obj;
extern unsigned char D_80157FA8[];
extern void func_800D8FE8(void *object);
void func_800DB234(Obj *obj, s32 flags) {
    obj->vtable_04 = D_80157FA8;
    if (flags & 1) func_800D8FE8(obj);
}
