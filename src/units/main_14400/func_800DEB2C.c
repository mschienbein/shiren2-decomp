#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad0[0xC8];
    s32 field_C8;
} Obj_800DEB2C;

s32 func_800DEB2C(Obj_800DEB2C *obj) {
    return obj->field_C8;
}
