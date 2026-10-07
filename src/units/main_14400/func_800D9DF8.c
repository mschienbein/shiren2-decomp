#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
extern u8 D_80157FA8[];
extern u8 D_80158128[];
typedef struct { u16 type; u16 pad; void *vt; } Obj;
Obj *func_800D9DF8(Obj *o) { o->vt = D_80157FA8; o->type = 0x31; o->vt = D_80158128; return o; }
