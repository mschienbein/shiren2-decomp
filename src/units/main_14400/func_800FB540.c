#include "common.h"
typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad0[0x24]; const VTable *vtable24; } Obj800EFC70;
extern void *func_800A38FC(s32 size);
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern const VTable D_8015A1F0;
Obj800EFC70 *func_800FB540(u8 value, Obj800EFC70 *object) {
    if (!object) object = func_800A38FC(0xA0);
    func_800EFC70(object, 0x22, value);
    object->vtable24 = &D_8015A1F0;
    return object;
}
