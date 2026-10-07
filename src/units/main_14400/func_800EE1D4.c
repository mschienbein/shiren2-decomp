#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x94];
    u8 flags94;
} Obj800EE1D4;

void func_800EE1D4(Obj800EE1D4 *obj) {
    obj->flags94 |= 2;
}
