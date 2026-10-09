#include "common.h"

typedef struct {
    unsigned char pad0[0x42];
    unsigned char field_42;
    unsigned char pad43[0x72 - 0x43];
    unsigned char field_72;
} Obj;

s32 func_800E1CD4(Obj *obj, s32 kind);

s32 func_800E2074(Obj *obj)
{
    s32 result = 0;

    if (obj->field_42 != 0 && func_800E1CD4(obj, 0x10) == 0) {
        return result;
    }
    if (!(obj->field_72 & 1)) {
        result = 1;
    }
    return result;
}
