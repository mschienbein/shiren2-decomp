#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x32];
    u8 field_32;
} Obj;

u8 func_800E3324(Obj *obj) {
    return obj->field_32;
}
