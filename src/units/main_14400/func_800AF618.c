#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x1];
    u8 field_1;
} Obj_800AF618;

u8 func_800AF618(Obj_800AF618 *obj) {
    return obj->field_1;
}
