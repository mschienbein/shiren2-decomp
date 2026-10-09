#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct Obj800EFC70 {
    u8 pad_00[0x24];
    VTable *vtable_24;
    u8 pad_28[0x4A];
    u8 flags_72;
} Obj800EFC70;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *object, s32 kind, u8 variant);
extern VTable D_8015B068;

Obj800EFC70 *func_800FFE00(Obj800EFC70 *object, u8 variant)
{
    func_800EFC70(object, 0x35, variant);
    object->vtable_24 = &D_8015B068;
    object->flags_72 |= 2;
    return object;
}
