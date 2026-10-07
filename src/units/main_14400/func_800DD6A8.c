#include "common.h"
typedef struct { unsigned char pad0[4]; void *vtable; } Obj800DD6A8;
extern s32 D_80157FA8;
void func_800D8FE8(void *obj);
void func_800DD6A8(Obj800DD6A8 *obj, s32 flags) {
    obj->vtable = &D_80157FA8;
    if (flags & 1) func_800D8FE8(obj);
}
