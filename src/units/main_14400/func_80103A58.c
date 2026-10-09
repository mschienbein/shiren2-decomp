#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Pair_80103A58;

typedef struct {
    char pad0[0xA0];
    Pair_80103A58 field_A0;
} Obj_80103A58;

void func_80103A58(Obj_80103A58 *obj, Pair_80103A58 *value) {
    obj->field_A0 = *value;
}
