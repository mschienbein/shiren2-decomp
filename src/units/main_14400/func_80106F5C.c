#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x8A];
    u8 field_8A;
} Obj_80106F5C;

u8 func_80106F5C(Obj_80106F5C *obj) {
    return obj->field_8A;
}
