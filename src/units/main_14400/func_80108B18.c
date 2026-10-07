#include "common.h"

typedef unsigned char u8;
typedef struct { char pad0[0xA0]; u8 field_A0; } Obj;

void func_80108B18(Obj *o, u8 v) {
    o->field_A0 = v;
}
