#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x8A];
    u8 field_8A;
} Obj;

u8 func_800FD91C(Obj *obj) {
    return obj->field_8A;
}
