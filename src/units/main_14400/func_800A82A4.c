#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 pad0[0x1C]; u16 flags1C; } Obj;
void func_800A82A4(Obj *obj) { obj->flags1C &= ~4; }
