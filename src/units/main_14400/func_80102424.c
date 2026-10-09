#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_0[0x24]; void *field_24; u8 pad_28[0x9A - 0x28]; u16 field_9A; } Obj;
extern Obj *func_800EFC70(Obj *obj, s32 arg1, u8 arg2);
extern void func_800E4D88(Obj *obj, s32 value);
extern void func_800E4D90(Obj *obj, s32 value);
extern u8 D_8015B838[];
Obj *func_80102424(Obj *object, u8 value)
{
    func_800EFC70(object, 64, value);
    object->field_24 = D_8015B838;
    func_800E4D88(object, 3);
    func_800E4D90(object, 2);
    object->field_9A |= 3;
    return object;
}
