#include "common.h"
typedef unsigned char u8;
typedef struct {
    u8 field_00[0xA]; u8 field_0A; u8 field_0B[0x14]; u8 field_1F;
    u8 field_20[4]; const void *field_24; u8 field_28[4];
} Object;
extern void *func_800A38FC(s32 size);
extern void *func_800F4760(Object *object);
extern const u8 D_80149610[];
Object *func_800F5EC0(void) {
    Object *object = func_800A38FC(0x2C);
    func_800F4760(object);
    object->field_24 = D_80149610;
    object->field_0A = 20;
    object->field_1F = 20;
    return object;
}
