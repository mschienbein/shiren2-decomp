#include "common.h"
typedef signed short s16;
typedef struct { s16 field_00; s16 field_02; s32 field_04; s32 field_08; const void *field_0C; } Object;
extern const unsigned char D_80149DF8[];
Object *func_800C4EC0(Object *object, s32 kind, s32 arg2, s32 arg3) {
    object->field_0C = D_80149DF8;
    object->field_00 = kind;
    object->field_04 = arg2;
    object->field_08 = arg3;
    return object;
}
