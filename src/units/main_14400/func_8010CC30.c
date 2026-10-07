#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0xC];
    u16 field_C;
} Obj;

u16 func_8010CC30(Obj *obj) {
    return obj->field_C;
}
