#include "common.h"
typedef unsigned short u16;
typedef struct { char pad[0x9A]; u16 field9A; } Obj;
s32 func_800F3B60(Obj *p) { s32 flags = p->field9A & 4; return flags != 0; }
