#include "common.h"
typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad_0[0x24]; VTable *field_24; } Obj800EFC70;
extern VTable D_8015BBF8;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
Obj800EFC70 *func_80105168(Obj800EFC70 *obj, u8 kind) {
    func_800EFC70(obj, 0x45, kind);
    obj->field_24 = &D_8015BBF8;
    return obj;
}
