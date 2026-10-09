#include "common.h"
typedef unsigned char u8;
typedef struct {
    u8 pad_0[0xA]; u8 field_A;
    u8 pad_B[0x1F - 0xB]; u8 field_1F;
    u8 pad_20[4]; void *field_24;
} Object;
extern void *func_800A38FC(s32 size);
extern void *func_800F4760(Object *object);
extern u8 D_80149130[];
Object *func_800F4D10(void)
{
    Object *object = func_800A38FC(0x2C);
    func_800F4760(object);
    object->field_24 = D_80149130;
    object->field_A = 2;
    object->field_1F = 2;
    return object;
}
