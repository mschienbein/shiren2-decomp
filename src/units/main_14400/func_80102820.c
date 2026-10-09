#include "common.h"
typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad0[0x24]; const VTable *vtable24; u8 pad28[0x78]; s32 fieldA0; s32 fieldA4; } Obj800EFC70;
extern void *func_800A38FC(s32 size);
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern const VTable D_8015B8F8;
Obj800EFC70 *func_80102820(u8 value, Obj800EFC70 *object) {
    if (!object) object = func_800A38FC(0xA8);
    func_800EFC70(object, 0x41, value);
    object->vtable24 = &D_8015B8F8;
    object->fieldA0 = 0;
    object->fieldA4 = 0;
    return object;
}
