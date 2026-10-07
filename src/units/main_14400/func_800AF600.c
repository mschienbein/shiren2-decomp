#include "common.h"

typedef unsigned char u8;
typedef struct { unsigned char pad0[0x3]; u8 field_3; } Obj;

u8 func_800AF600(Obj *arg0) {
    return arg0->field_3;
}
