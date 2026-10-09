#include "common.h"

typedef struct VTable VTable;

typedef struct {
    char pad00[0x24];
    VTable *vtable;
} Obj80136734;

/* Opaque mixed adjustment/function-pointer table; only its address is used here. */
extern VTable D_80149BA8;
extern void func_800EFD28(Obj80136734 *obj, s32 flags);
extern void func_800A3918(Obj80136734 *obj);

/* Deleting destructor: restore vtable, run the base destructor, free on bit 0. */
void func_80136734(Obj80136734 *obj, s32 flags) {
    obj->vtable = &D_80149BA8;
    func_800EFD28(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
