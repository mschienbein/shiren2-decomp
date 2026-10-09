#include "common.h"

typedef struct {
    unsigned char fields_00[0x42];
    unsigned char field_42;
} Object;

extern s32 func_800E1CD4(Object *, s32);

s32 func_800E1D54(Object *object) {
    s32 result = 0;
    if (func_800E1CD4(object, 0xD) != 0) {
        result = object->field_42 == 0xFF;
    }
    return result;
}
