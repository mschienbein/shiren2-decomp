#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad[0x94]; u8 field94; } Obj;
s32 func_800EE1E4(Obj *p) { return p->field94 & 1; }
