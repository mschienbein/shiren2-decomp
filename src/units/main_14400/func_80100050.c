#include "common.h"

typedef unsigned char u8;

typedef struct {
    unsigned char pad0[0x24];
    void *vtable;
    unsigned char pad28[0x9A - 0x28];
    unsigned short field_9A;
} Unit;

extern unsigned char D_8015B128[];
Unit *func_800EFC70(Unit *obj, s32 arg1, u8 arg2);
void func_800E4D88(Unit *obj, s32 value);
void func_800E4D90(Unit *a, s32 b);

Unit *func_80100050(Unit *obj, u8 kind)
{
    func_800EFC70(obj, 0x36, kind);
    obj->vtable = D_8015B128;
    func_800E4D88(obj, 3);
    func_800E4D90(obj, 2);
    obj->field_9A |= 1;
    return obj;
}
