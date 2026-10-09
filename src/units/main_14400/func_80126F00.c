#include "common.h"

typedef struct VTable VTable;
typedef struct Object {
    unsigned char pad_00[8];
    VTable *vtable_08;
    s32 field_0C;
} Object;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Object *func_801128F0(Object *object, s32 kind);
extern VTable D_80160568;

Object *func_80126F00(void)
{
    Object *object = func_800AC5B4(0x10, 0);
    func_801128F0(object, 0xEA);
    object->vtable_08 = &D_80160568;
    return object;
}
