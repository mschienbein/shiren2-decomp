#include "common.h"
typedef struct { unsigned char pad00[8]; const void *vtable; s32 field0C; } Object;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Object *func_801128F0(Object *object, s32 kind);
extern const unsigned char D_801605F8[72];
Object *func_80127460(void) {
    Object *object = func_800AC5B4(0x10, 0);
    func_801128F0(object, 0xEC);
    object->vtable = D_801605F8;
    return object;
}
