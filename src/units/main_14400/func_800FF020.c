#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x24]; const void *field_24; } Obj800EFC70;
extern const unsigned char D_8015AD68[192];
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
Obj800EFC70 *func_800FF020(Obj800EFC70 *object, u8 value)
{
    func_800EFC70(object, 49, value);
    object->field_24 = D_8015AD68;
    return object;
}
