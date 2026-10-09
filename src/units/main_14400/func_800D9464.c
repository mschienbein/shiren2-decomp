#include "common.h"

/* Only the vtable pointer at offset 4 is used here; the preceding word is opaque. */
typedef struct Object800D9464 {
    u32 opaque_00;
    void *vtable_04;
} Object800D9464;

/* Initialized original base vtable, not BSS. Its full type is unresolved. */
extern unsigned char D_80157FA8[];

void func_800D8FE8(void *object);

/* Deleting destructor: restore the base vtable, free when bit 0 of flags is set. */
void func_800D9464(Object800D9464 *object, s32 flags)
{
    object->vtable_04 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(object);
    }
}
