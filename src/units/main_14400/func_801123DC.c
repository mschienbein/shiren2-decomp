#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0xC]; u8 field_C; } Obj801123DC;

u8 func_801123DC(Obj801123DC *obj) {
    return obj->field_C;
}
