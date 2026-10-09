#include "common.h"
typedef struct VTable VTable;
typedef struct { unsigned char pad_0[8]; VTable *field_8; } Obj;
extern VTable D_80153AA0;
extern void func_800AC68C(void *a);
void func_80135FE0(Obj *obj, s32 flags) {
    obj->field_8 = &D_80153AA0;
    if (flags & 1) func_800AC68C(obj);
}
