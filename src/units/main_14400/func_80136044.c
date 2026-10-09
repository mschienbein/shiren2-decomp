#include "common.h"

/* Only the vtable pointer at offset 8 is used here. */
typedef struct Object80136044 {
    u32 opaque_00;
    u32 opaque_04;
    void *vtable_08;
} Object80136044;

/* Initialized original base vtable, not BSS. Its full type is unresolved. */
extern unsigned char D_80153AA0[];

void func_800AC68C(void *a);

/* Deleting destructor: restore the base vtable, free when bit 0 of flags is set. */
void func_80136044(Object80136044 *object, s32 flags)
{
    object->vtable_08 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(object);
    }
}
