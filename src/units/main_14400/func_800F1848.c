#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef struct { u8 field_00; u8 field_01; u8 pad_02[3]; s8 field_05; } Object;

/* self is supplied by virtual callers but unused by this predicate. */
s32 func_800F1848(void *self, Object *object) {
    if (object != 0 && ~object->field_05 == 0) {
        s32 type = object->field_00;
        u8 kind = type;
        s32 result = 0;
        if ((u32)(type - 15) >= 2) {
            if (kind != 19 || object->field_01 == 0xF2) {
                result = 1;
            }
        }
        return result;
    }
    return 0;
}
