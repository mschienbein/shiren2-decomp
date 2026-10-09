#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad00[0x24];
    void **vtable24;
} Obj800EFC70;

extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern void *D_8015AEE8[];

Obj800EFC70 *func_800FFC64(Obj800EFC70 *obj, u8 arg)
{
    func_800EFC70(obj, 0x33, arg);
    obj->vtable24 = D_8015AEE8;
    return obj;
}
