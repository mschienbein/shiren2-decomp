#include "common.h"

typedef unsigned short u16;
typedef struct { char pad0[0x30]; u16 field_30; } Obj;

u16 func_800E332C(Obj *o) {
    return o->field_30;
}
