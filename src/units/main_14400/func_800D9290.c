#include "common.h"
typedef struct VTable VTable;
typedef struct { unsigned char pad_0[4]; VTable *field_4; } Obj;
extern VTable D_80157FA8;
extern void func_800D8FE8(void *object);
void func_800D9290(Obj *obj, s32 flags) {
    obj->field_4 = &D_80157FA8;
    if (flags & 1) func_800D8FE8(obj);
}
