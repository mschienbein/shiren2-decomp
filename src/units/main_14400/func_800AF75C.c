#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[2]; u8 unk2; } Obj;
void func_800AF75C(Obj *obj) { obj->unk2 |= 4; }
