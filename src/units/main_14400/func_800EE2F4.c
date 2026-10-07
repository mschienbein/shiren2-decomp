#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad[0xE4]; u16 flags; } Obj;
u32 func_800EE2F4(Obj *o) { return (o->flags >> 2) & 1; }
