#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad[2]; u8 field2; } Obj;
void func_800AF790(Obj *p, s32 flags) { p->field2 |= flags; }
