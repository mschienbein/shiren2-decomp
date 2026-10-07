#include "common.h"

typedef unsigned char u8;

typedef struct { s32 unk0; s32 unk4; s32 unk8; } Entry;
typedef struct { u8 pad0[0x50]; Entry *entries; } Obj;
s32 func_80097630(Obj *obj, s32 index) { return obj->entries[index].unk8; }
