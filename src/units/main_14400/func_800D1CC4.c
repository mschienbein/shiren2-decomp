#include "common.h"
typedef unsigned char u8;
/* Stream read slot +0x28/+0x2C: void(receiver, s32 count, destination); targets func_800CA668/func_80044154/func_800444A4. */
typedef struct { u8 pad0[0x28]; short adjust_28; short pad2A; void (*read_2C)(void *, s32, void *); } Methods;
typedef struct { u8 pad0[0x18]; Methods *field_18; } Obj;
typedef struct { u8 field_0[0x14]; u8 field_14[0x14]; u8 pad28[0x93]; u8 field_BB[0xC]; } Data;
extern const char D_8015472C[];
extern void func_800CA4E8(Obj *, void *);
void func_800D1CC4(Data *data, Obj *obj) {
    func_800CA4E8(obj, (void *)D_8015472C);
    obj->field_18->read_2C((u8 *)obj + obj->field_18->adjust_28, 0x14, data->field_0);
    obj->field_18->read_2C((u8 *)obj + obj->field_18->adjust_28, 0x14, data->field_14);
    obj->field_18->read_2C((u8 *)obj + obj->field_18->adjust_28, 0xC, data->field_BB);
}
