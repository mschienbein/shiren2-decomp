#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x9F];
    u8 field_9F;
} Obj_800F39E8;

u8 func_800F39E8(Obj_800F39E8 *obj) {
    return obj->field_9F;
}
