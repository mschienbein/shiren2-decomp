#include "common.h"
typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad_0[0x20]; u32 field_20; VTable *field_24; u8 pad_28[0x68]; u32 field_90; u8 pad_94[0xC]; void *field_A0; } Obj800EFC70;
extern VTable D_8015A7C0;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
Obj800EFC70 *func_800FD460(Obj800EFC70 *obj, u8 kind) {
    func_800EFC70(obj, 0x2A, kind);
    obj->field_24 = &D_8015A7C0;
    obj->field_A0 = 0;
    obj->field_20 = (obj->field_90 |= 0x2000000);
    return obj;
}
