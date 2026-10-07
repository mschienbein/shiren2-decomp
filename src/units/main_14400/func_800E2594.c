#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x72]; u8 unk72; } Obj;
s32 func_800E2594(Obj *obj) { return obj->unk72 & 1; }
