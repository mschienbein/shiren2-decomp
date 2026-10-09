#include "common.h"

typedef struct VTable VTable;

typedef struct {
    u32 opaque_00;
    u32 opaque_04;
    u32 opaque_08;
    VTable *vtable_0C;
} Obj80136884;

/* Opaque mixed adjustment/function-pointer table; only its address is used here. */
extern VTable D_80149DC8;

void func_800D8FA8(void *object);

void func_80136884(Obj80136884 *obj, s32 flags) {
    obj->vtable_0C = &D_80149DC8;
    if (flags & 1) {
        func_800D8FA8(obj);
    }
}
