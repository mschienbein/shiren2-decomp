#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad_00[0xD]; u8 field_0D; } Object;

s32 func_80113FD8(Object *object) {
    s32 result = 0;
    if (object->field_0D & 2) {
        result = 1;
    }
    return result;
}
