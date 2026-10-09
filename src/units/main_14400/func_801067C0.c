#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct Obj800EFC70 {
    u8 pad_00[0x24];
    VTable *vtable_24;
} Obj800EFC70;
extern void *func_800A38FC(s32 size);
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *object, s32 kind, u8 variant);
extern VTable D_80149A38;

Obj800EFC70 *func_801067C0(u8 variant, Obj800EFC70 *object)
{
    if (!object)
        object = func_800A38FC(0xA0);
    func_800EFC70(object, 0x4B, variant);
    object->vtable_24 = &D_80149A38;
    return object;
}
