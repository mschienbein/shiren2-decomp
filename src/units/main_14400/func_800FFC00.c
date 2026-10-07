#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8A];
    u8 field_8A;
} Obj_800FFC00;

u8 func_800FFC00(Obj_800FFC00 *obj) {
    return obj->field_8A;
}
