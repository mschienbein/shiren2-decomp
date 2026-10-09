#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x24];
    void *vtable;
} Obj80103E38;

extern u8 D_8015BA78[];

Obj80103E38 *func_800EFC70(Obj80103E38 *obj, s32 arg1, u8 arg2);

/* Constructor: build the base entity of kind 0x43, then install this class's vtable. */
Obj80103E38 *func_80103E38(Obj80103E38 *obj, u8 arg)
{
    func_800EFC70(obj, 0x43, arg);
    obj->vtable = D_8015BA78;
    return obj;
}
