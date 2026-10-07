#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0x52];
    u8 field_52;
} Obj;

u8 func_800E23A4(Obj *obj) {
    return obj->field_52;
}
