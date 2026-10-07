#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x80]; u8 field_80; } Obj;
u8 func_800F3CE8(Obj *o) { return o->field_80; }
