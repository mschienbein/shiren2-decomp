#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x24];
    void *field_24;
    u8 pad28[0x9A - 0x28];
    u16 field_9A;
    u8 pad9C[0xA0 - 0x9C];
    s32 field_A0;
    s32 field_A4;
    s32 field_A8;
} Obj_80107000;

extern u8 D_8015C208[];
extern void *func_800EFC70(Obj_80107000 *obj, s32 kind, u8 mode);
extern u8 func_801E9F70(s32 flag);

Obj_80107000 *func_80107000(Obj_80107000 *obj, u8 mode) {
    func_800EFC70(obj, 0x50, mode);
    obj->field_24 = D_8015C208;
    if (func_801E9F70(0xE)) {
        if (mode == 3) {
            obj->field_A0 = 2;
        } else {
            obj->field_A0 = 1;
        }
    } else {
        obj->field_A0 = 0;
    }
    obj->field_A4 = 0;
    obj->field_A8 = 0;
    if (mode == 3) {
        obj->field_9A |= 3;
    }
    return obj;
}
