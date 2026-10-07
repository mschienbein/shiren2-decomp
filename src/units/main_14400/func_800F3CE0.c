#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x7E];
    u16 field_7E;
} Obj_800F3CE0;

u16 func_800F3CE0(Obj_800F3CE0 *obj) {
    return obj->field_7E;
}
