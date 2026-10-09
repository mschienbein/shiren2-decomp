#include "common.h"

/* Only the vtable pointer at offset 4 is touched; the preceding word is opaque. */
typedef struct {
    u32 opaque_00;
    void *vtable_04;
} Obj800D9FD4;

/* Initialized original vtable; its full type is unresolved. */
extern unsigned char D_80157FA8[];

extern void func_800D8FE8(void *object);

void func_800D9FD4(Obj800D9FD4 *obj, s32 flags) {
    obj->vtable_04 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
