#include "common.h"
extern s32 D_80151E10[];
typedef struct { s32 *field_00; unsigned char pad_04[0xC]; s32 field_10; } Object;
Object *func_800953A0(Object *object) {
    object->field_00 = D_80151E10;
    object->field_10 = -1;
    return object;
}
