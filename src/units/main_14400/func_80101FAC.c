#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { unsigned char pad00[0x24]; VTable *vtable; } Obj800EFC70;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern VTable D_8015B748;

Obj800EFC70 *func_80101FAC(Obj800EFC70 *obj, u8 value)
{
    func_800EFC70(obj, 0x3F, value);
    obj->vtable = &D_8015B748;
    return obj;
}
