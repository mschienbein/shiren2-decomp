#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[3];
    u8 field_3;
} Obj800AF5F8;

u8 func_800AF5F8(Obj800AF5F8 *obj) {
    return obj->field_3;
}
