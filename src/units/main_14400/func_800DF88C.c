#include "common.h"

/* Only the pointer at offset 4 is used here; the preceding word is opaque. */
typedef struct Object800DF88C {
    u32 opaque_00;
    void *field_04;
} Object800DF88C;

/* Initialized original table, not BSS. Its full type is unresolved. */
extern unsigned char D_80157FA8[];

/* The original routine is empty but accepts the caller's object pointer. */
extern void func_800D8FE8(void *object);

void func_800DF88C(Object800DF88C *object, s32 flags)
{
    object->field_04 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(object);
    }
}
