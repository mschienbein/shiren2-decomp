#include "common.h"

typedef unsigned char u8;
typedef struct { char pad0[0x94]; u8 flags94; } Obj;
void func_800EE1A4(Obj *obj) {
    obj->flags94 |= 4;
}
