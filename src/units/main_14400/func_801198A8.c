#include "common.h"

typedef struct VTable VTable;

/* Only the vtable pointer at offset 8 is used here. */
typedef struct Object801198A8 {
    u32 opaque_00;
    u32 opaque_04;
    const VTable *vtable_08;
} Object801198A8;

/* Original base vtable D_80153AA0 (rodata: adjustment words and function pointers),
 * modeled as one opaque whole object; only its address is stored here. */
extern const VTable D_80153AA0;

void func_800AC68C(void *a);

/* Deleting destructor: restore the base vtable, free when bit 0 of flags is set. */
void func_801198A8(Object801198A8 *object, s32 flags)
{
    object->vtable_08 = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(object);
    }
}
