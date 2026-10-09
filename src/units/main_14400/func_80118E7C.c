#include "common.h"

typedef unsigned char u8;

/* Original base vtable D_80153AA0 (rodata: adjustment words and function pointers),
 * modeled as one opaque whole object; only its address is stored here. */
typedef struct VTable VTable;

typedef struct {
    u8 pad0[0x8];
    const VTable *vtable;
} Obj80118E7C;

extern const VTable D_80153AA0;

void func_800AC68C(void *a);

/* Destructor (vtable slot 1): restore the base vtable, free when bit 0 of `flags` is set. */
void func_80118E7C(Obj80118E7C *obj, s32 flags)
{
    obj->vtable = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
