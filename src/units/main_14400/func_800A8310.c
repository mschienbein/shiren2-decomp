#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad[0x1C]; u16 flags; } Obj;
void func_800A8310(Obj *o) { o->flags |= 1; }
