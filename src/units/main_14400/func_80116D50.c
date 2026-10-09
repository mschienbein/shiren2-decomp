#include "common.h"
typedef struct { unsigned char pad0[8]; void *vtable8; } Object;
extern unsigned char D_8015DA98[];
extern void *func_800AC0C0(void *object, s32 kind, s32 value);
Object *func_80116D50(Object *object, s32 kind) {
    func_800AC0C0(object, 1, kind);
    object->vtable8 = D_8015DA98;
    return object;
}
