#include "common.h"
typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad_0[0xA]; u8 field_A; u8 pad_B[0x14]; u8 field_1F; u32 field_20; VTable *field_24; u32 field_28; } Object;
extern VTable D_801492D0;
extern void *func_800A38FC(s32 size);
extern void *func_800F4760(Object *obj);
Object *func_800F4E50(void) {
    Object *obj = func_800A38FC(0x2C);
    func_800F4760(obj);
    obj->field_24 = &D_801492D0;
    obj->field_A = 6;
    obj->field_1F = 6;
    return obj;
}
