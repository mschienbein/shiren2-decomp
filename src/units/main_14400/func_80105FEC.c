#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct Obj800EFC70 { u8 pad_00[0x24]; VTable *field_24; } Obj800EFC70;
extern VTable D_8015BE38;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);

Obj800EFC70 *func_80105FEC(Obj800EFC70 *object, u8 value) {
    func_800EFC70(object, 0x48, value);
    object->field_24 = &D_8015BE38;
    return object;
}
