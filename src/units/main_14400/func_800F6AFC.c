#include "common.h"

typedef struct {
    char pad0[0xA0];
    s32 field_A0;
    s32 field_A4;
} Obj;

s32 func_800F6AFC(Obj *obj) {
    return obj->field_A0 != 0 || obj->field_A4 != 0;
}
