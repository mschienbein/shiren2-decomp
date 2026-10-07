#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;

typedef struct { u8 x_0; u8 y_1; u8 pad2[0xD]; u8 value_F; } Obj8010BC54;
s32 func_800A2910(u8 x, u8 y, u16 *out);

s8 func_8010BC54(Obj8010BC54 *obj) {
    u16 buf[2];
    s32 value = obj->value_F;

    if (func_800A2910(obj->x_0, obj->y_1, buf)) {
        value++;
    }
    return value;
}
