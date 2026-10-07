#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x75];
    u8 field_75;
} Obj800E331C;

u8 func_800E331C(Obj800E331C *obj) {
    return obj->field_75;
}
