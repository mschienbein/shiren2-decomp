#include "common.h"
typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad0[0x24]; VTable *vtable; } Object;
extern VTable D_80159440;
extern u32 D_8013960C;
extern s32 func_800F0440(Object *object);
extern void func_800E016C(Object *object, s32 flags);
extern void func_800A3918(Object *object);
void func_800EFD28(Object *object, s32 flags) {
    object->vtable = &D_80159440;
    D_8013960C <<= 1;
    func_800F0440(object);
    D_8013960C >>= 1;
    func_800E016C(object, 0);
    if (flags & 1) func_800A3918(object);
}
