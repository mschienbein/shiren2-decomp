#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 pad0[0x1C]; u16 flags1C; } Obj;
s32 func_800A82F4(Obj *obj) { return obj->flags1C & 1; }
