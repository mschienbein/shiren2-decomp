#include "common.h"

typedef unsigned char u8;
typedef struct Obj800EFC70 { u8 pad_00[0x24]; const void *field_24; u8 pad_28[0x72]; unsigned short field_9A; } Obj800EFC70;
typedef Obj800EFC70 Obj;
typedef Obj800EFC70 S;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern void func_800E4D88(Obj *obj, s32 value);
extern void func_800E4D90(S *a, s32 b);
extern const u8 D_8015A470[];
Obj800EFC70 *func_800FC4E0(Obj800EFC70 *obj, u8 value)
{
    func_800EFC70(obj, 0x26, value);
    obj->field_24 = D_8015A470;
    func_800E4D88(obj, 3);
    func_800E4D90(obj, 2);
    obj->field_9A |= 7;
    return obj;
}
