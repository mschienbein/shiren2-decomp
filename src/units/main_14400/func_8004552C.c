#include "common.h"
typedef struct { unsigned char *field_00; unsigned char pad_04[0x10]; s32 field_14; } Object;
extern s32 func_80045608(Object *object, s32 delta, s32 value);
s32 func_8004552C(Object *object, s32 value) {
    s32 delta = value - object->field_14;
    s32 result = (unsigned short)func_80045608(object, delta, object->field_00[1]);
    if (delta < 0) result = -result;
    result += object->field_00[1];
    if (result > 255) result = 255;
    if (result < 0) result = 0;
    return result;
}
