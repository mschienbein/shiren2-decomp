#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct {
    u8 pad_00[0xA];
    u8 field_0A;
    u8 pad_0B[0x14];
    u8 field_1F;
    u8 pad_20[4];
    const VTable *field_24;
    u8 pad_28[4];
} Object;
extern const VTable D_801596F0;
extern void *func_800A38FC(s32 size);
extern void *func_800F4760(Object *obj);

Object *func_800F51A0(void) {
    Object *obj = func_800A38FC(0x2C);
    func_800F4760(obj);
    obj->field_24 = &D_801596F0;
    obj->field_0A = 10;
    obj->field_1F = 10;
    return obj;
}
