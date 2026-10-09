#include "common.h"

typedef unsigned char u8;

typedef struct {
    unsigned char pad0[0x24];
    void *vtable;
} Unit;

extern unsigned char D_8015A530[];
Unit *func_800EFC70(Unit *obj, s32 arg1, u8 arg2);

Unit *func_800FCA08(Unit *obj, u8 kind)
{
    func_800EFC70(obj, 0x27, kind);
    obj->vtable = D_8015A530;
    return obj;
}
