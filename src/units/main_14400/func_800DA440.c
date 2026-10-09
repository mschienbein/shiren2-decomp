#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* Only the vtable pointer at offset 4 is used here; the preceding word is opaque. */
typedef struct {
    u32 opaque_00;
    void *vtable_04;
} Obj800DA440;

/* Initialized original vtable; its full type is unresolved. */
extern unsigned char D_80157FA8[];

/* The original routine is empty but accepts the caller's object pointer. */
extern void func_800D8FE8(void *object);

void func_800DA440(Obj800DA440 *obj, s32 flags)
{
    obj->vtable_04 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
