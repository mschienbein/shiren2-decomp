#include "common.h"

typedef unsigned short u16;
typedef struct { char pad0[0x2A]; u16 field_2A; } Obj;
u16 func_800E336C(Obj *o) { return o->field_2A; }
