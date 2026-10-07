#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 field_0; u8 field_1; u8 field_2; u8 pad3[0x9]; u8 field_C; } Item800B502C;
Item800B502C *func_800B4D80(void *obj);
void *func_800B31E8(void *obj, s32 kind);
s32 func_800B502C(void *obj) {
    Item800B502C *item = func_800B4D80(obj);
    s32 result = 0;
    if (item != 0 && item->field_1 == 0x26 && (item->field_2 & 0x20) && !(item->field_C & 1)) {
        result = func_800B31E8(obj, 0xA) == 0;
    }
    return result;
}
