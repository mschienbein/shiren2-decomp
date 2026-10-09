#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0x24]; void *field_24; u8 pad28[0x78]; } Obj800EFC70;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern void *func_800A38FC(s32 size);
extern u8 D_801498C8[];
Obj800EFC70 *func_800FC440(u8 value, Obj800EFC70 *obj) {
    if (!obj) obj = func_800A38FC(0xA0);
    func_800EFC70(obj, 0x25, value);
    obj->field_24 = D_801498C8;
    return obj;
}
