#include "common.h"

typedef unsigned char u8;
typedef struct Obj800EFC70 { u8 pad_00[0x24]; const void *field_24; } Obj800EFC70;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern const unsigned char D_8015A2B0[192];
Obj800EFC70 *func_800FBD48(Obj800EFC70 *obj, u8 value)
{
    func_800EFC70(obj, 0x23, value);
    obj->field_24 = D_8015A2B0;
    return obj;
}
