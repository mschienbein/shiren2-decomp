#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x94]; u8 flags94; } Obj;
void func_800EE228(Obj *obj) { obj->flags94 |= 8; }
