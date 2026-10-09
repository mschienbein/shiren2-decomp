#include "common.h"

typedef struct { s32 field_0; const void *vtable_4; } Obj800DC9A4;
extern const s32 D_80157FA8[];
void func_800D8FE8(Obj800DC9A4 *obj);

void func_800DC9A4(Obj800DC9A4 *obj, s32 flags) {
    obj->vtable_4 = &D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
