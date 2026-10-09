#include "common.h"
typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad0[0x24]; const VTable *vtable24; } Obj800EFC70;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern void func_800FDA04(Obj800EFC70 *object);
extern const VTable D_8015A880;
Obj800EFC70 *func_800FD9C0(Obj800EFC70 *object, u8 value) {
    func_800EFC70(object, 0x2B, value);
    object->vtable24 = &D_8015A880;
    func_800FDA04(object);
    return object;
}
