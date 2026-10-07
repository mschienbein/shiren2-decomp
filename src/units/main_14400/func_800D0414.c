#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad0[0x10];
    s32 field_10;
} Obj_800D0414;

s32 func_800D0414(Obj_800D0414 *obj) {
    return obj->field_10;
}
