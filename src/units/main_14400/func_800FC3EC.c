#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x24];
    void *vtable;
} Obj800FC3EC;

extern u8 D_8015A3A8[];

Obj800FC3EC *func_800EFC70(Obj800FC3EC *obj, s32 arg1, u8 arg2);
void func_800E4D88(Obj800FC3EC *obj, s32 value);

/* Constructor: base entity of kind 0x24, this class's vtable, then state 1. */
Obj800FC3EC *func_800FC3EC(Obj800FC3EC *obj, u8 level)
{
    func_800EFC70(obj, 0x24, level);
    obj->vtable = D_8015A3A8;
    func_800E4D88(obj, 1);
    return obj;
}
