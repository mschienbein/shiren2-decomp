#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[12]; u8 field_0C; } Object;
extern u8 D_801573F8[], D_80157420[];
extern u8 func_800AE98C(u8 *object);
extern void func_8010BC98(u8 *object, u8 value);
void func_8010EAA8(Object *object) {
    s32 kind = func_800AE98C((u8 *)object);
    s32 flags = D_801573F8[kind];
    s32 value = D_80157420[kind];
    object->field_0C = flags;
    func_8010BC98((u8 *)object, value);
}
