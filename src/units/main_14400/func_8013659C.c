#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;

typedef struct S {
    u8 pad0[0x24];
    VTable *vtable;
} S;

/* Opaque mixed adjustment/function-pointer table; only its address is used here. */
extern VTable D_80149810;

void func_800EFD28(S *, s32);
void func_800A3918(S *obj);

/* Destructor (vtable slot 1): restore this vtable, run the base destructor, free on bit 0. */
void func_8013659C(S *obj, s32 flags)
{
    obj->vtable = &D_80149810;
    func_800EFD28(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
