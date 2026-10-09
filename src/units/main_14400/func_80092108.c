#include "common.h"

typedef struct {
    u32 opaque_00;
    u32 opaque_04;
    u32 opaque_08;
    void *vtable_0C;
    u32 opaque_10;
    u32 opaque_14;
    char member_18[0x10];
} Obj80092108;

/* Initialized original vtable; its full type is unresolved. */
extern unsigned char D_80151350[];

void func_800922B0(void *object, s32 flags);
void func_800D8FA8(void *object);

void func_80092108(Obj80092108 *obj, s32 flags) {
    func_800922B0(obj->member_18, 2);
    obj->vtable_0C = D_80151350;
    if (flags & 1) {
        func_800D8FA8(obj);
    }
}
