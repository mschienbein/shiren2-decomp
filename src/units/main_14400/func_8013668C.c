#include "common.h"

typedef struct VTable VTable;

/* Partial view: the derived vtable pointer lives at 0x24. */
typedef struct Obj8013668C {
    unsigned char pad_00[0x24];
    VTable *vtable_24;
} Obj8013668C;

/* Opaque mixed adjustment/function-pointer table; only its address is used here. */
extern VTable D_80149A38;

void func_800EFD28(void *obj, s32 flags);
void func_800A3918(void *obj);

/* Deleting destructor: restore this class's vtable, run the base destructor without
 * freeing, then free when bit 0 of flags is set. */
void func_8013668C(Obj8013668C *obj, s32 flags)
{
    obj->vtable_24 = &D_80149A38;
    func_800EFD28(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
