#include "common.h"
typedef unsigned char u8;
typedef struct { u8 field_00[0x4C]; const void *field_4C; } Base;
typedef struct {
    Base field_00; u8 field_50[0xC8]; const void *field_118;
    u8 field_11C[0x54]; Base field_170; u8 field_1C0[0x58];
    const void *field_218; u8 field_21C[0x124]; const void *field_340;
} Object;
extern const unsigned char D_80152A50[144], D_80151E38[144], D_80152968[152];
extern void func_800D8FA8(void *object);
void func_8009D450(Object *object, s32 flags) {
    Base *inner = &object->field_170;
    object->field_00.field_4C = D_80152A50;
    object->field_340 = D_80151E38;
    inner->field_4C = D_80152968;
    object->field_218 = D_80151E38;
    inner->field_4C = D_80151E38;
    object->field_118 = D_80151E38;
    object->field_00.field_4C = D_80151E38;
    if (flags & 1) func_800D8FA8(object);
}
