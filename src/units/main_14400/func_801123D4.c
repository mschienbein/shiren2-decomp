#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0xD]; u8 field_D; } Obj801123D4;

u8 func_801123D4(Obj801123D4 *obj) {
    return obj->field_D;
}
