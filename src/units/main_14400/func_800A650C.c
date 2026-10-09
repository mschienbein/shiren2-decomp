#include "common.h"
typedef struct { s32 field_00; s32 field_04; } Vec;
extern s32 func_800A23E8(Vec *origin, Vec *vector);
s32 func_800A650C(Vec *origin, Vec *vector) {
    Vec copy;
    copy.field_00 = vector->field_00;
    {
        s32 y = vector->field_04;
        vector = &copy;
        vector->field_04 = y;
    }
    return func_800A23E8(origin, vector);
}
