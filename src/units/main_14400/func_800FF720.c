#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad00[0x24]; void *vtable_24; u8 pad28[0x78]; } Object;
extern void *func_800A38FC(s32);
extern Object *func_800EFC70(Object *, s32, u8);
extern u8 D_8015AEE8[];
Object *func_800FF720(u8 kind, Object *object)
{
    if (object == 0) {
        object = func_800A38FC(0xA0);
    }
    func_800EFC70(object, 0x33, kind);
    object->vtable_24 = D_8015AEE8;
    return object;
}
