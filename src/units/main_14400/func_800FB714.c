#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x24];
    const void *vtable;
} Obj800EFC70;

extern const unsigned char D_8015A1F0[];

Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);

Obj800EFC70 *func_800FB714(Obj800EFC70 *obj, u8 arg1)
{
    func_800EFC70(obj, 0x22, arg1);
    obj->vtable = D_8015A1F0;
    return obj;
}
