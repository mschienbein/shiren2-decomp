#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xD6];
    u8 field_D6;
} Obj_801296AC;

u8 *func_801296AC(Obj_801296AC *obj, u8 *cursor) {
    obj->field_D6 = 1;
    return cursor;
}
