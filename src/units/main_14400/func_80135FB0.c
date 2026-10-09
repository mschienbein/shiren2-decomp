#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8];
    void *vtable;
} Obj80135FB0;

extern u8 D_80153AA0[];

void func_800AC68C(void *a);

/* Destructor (vtable slot 1): restore the base vtable, free when bit 0 of `flags` is set. */
void func_80135FB0(Obj80135FB0 *obj, s32 flags)
{
    obj->vtable = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
