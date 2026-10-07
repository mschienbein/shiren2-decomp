#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x20]; s32 field_20; u8 pad24[0x18]; s32 field_3C; } Obj800C47A8;
s32 func_800C47A8(Obj800C47A8 *obj) {
    s32 value = obj->field_3C;
    if (value == 0) {
        value = obj->field_20;
    }
    return value;
}
