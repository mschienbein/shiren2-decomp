#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x9F]; u8 field_9F; } Obj;
void func_800F39F0(Obj *o, u8 v) { o->field_9F = v; }
