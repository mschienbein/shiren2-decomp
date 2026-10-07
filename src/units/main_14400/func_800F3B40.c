#include "common.h"

typedef unsigned char u8;

typedef unsigned short u16;
typedef struct { u8 pad0[0x9A]; u16 unk9A; } Obj;
void func_800F3B40(Obj *obj) { obj->unk9A &= ~8; }
