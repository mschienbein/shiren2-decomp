#include "common.h"

typedef unsigned char u8;

typedef struct {
    short kind;
    u8 pad2[2];
    void *vtable;
} Obj800D9084;

extern u8 D_80157FA8[];

void func_800D8FE8(void *object);

/* Destructor (vtable slot 1): restore the base vtable, free when bit 0 of `flags` is set. */
void func_800D9084(Obj800D9084 *obj, s32 flags)
{
    obj->vtable = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
