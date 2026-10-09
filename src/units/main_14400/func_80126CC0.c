#include "common.h"

typedef struct { s32 pad00[2]; const void *vtable08; s32 field0C; } Object;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Object *func_801128F0(Object *object, s32 kind);
extern const unsigned char D_80160520[72];

Object *func_80126CC0(void)
{
    Object *object = func_800AC5B4(0x10, 0);
    func_801128F0(object, 0xE9);
    object->vtable08 = D_80160520;
    return object;
}
