#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0x24]; void *vtable_24; } Obj;
extern void *func_800A38FC(s32 size);
extern Obj *func_800EFC70(Obj *obj, s32 kind, u8 arg);
extern unsigned char D_80149AF0[];
Obj *func_80106820(u8 arg, Obj *obj) {
    if (obj == 0) obj = func_800A38FC(0xA0);
    func_800EFC70(obj, 0x4C, arg);
    obj->vtable_24 = D_80149AF0;
    return obj;
}
