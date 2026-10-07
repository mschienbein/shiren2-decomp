#include "common.h"

typedef struct { char pad0[0x8C]; s32 field_8C; } Obj;

s32 *func_800F8384(Obj *o) {
    return &o->field_8C;
}
