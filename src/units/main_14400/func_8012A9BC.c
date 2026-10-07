#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x10]; s32 unk10; } Obj;
void func_8012A9BC(Obj *obj, s32 value) { obj->unk10 = value; }
