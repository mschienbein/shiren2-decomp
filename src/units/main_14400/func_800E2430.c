#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad_00[0x54]; u8 field_54; } Object;

s32 func_800E2430(Object *object) {
    s32 result = 0;
    if (object->field_54 & 8) {
        result = 1;
    }
    return result;
}
