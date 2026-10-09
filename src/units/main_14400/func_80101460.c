#include "common.h"
typedef unsigned char u8;
typedef struct {
    u8 pad0[0x20]; u32 field_20; void *field_24;
    u8 pad28[0x68]; u32 field_90;
} Obj800EFC70;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern u8 D_80149980[];
Obj800EFC70 *func_80101460(Obj800EFC70 *obj, u8 value) {
    func_800EFC70(obj, 0x3C, value);
    obj->field_24 = D_80149980;
    obj->field_90 |= 0x8000;
    obj->field_20 = obj->field_90;
    return obj;
}
