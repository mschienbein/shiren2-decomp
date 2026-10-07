#include "common.h"
typedef struct { unsigned char pad[2]; unsigned char unk2; } S;
s32 func_800AF6F0(S *s) { u32 v = s->unk2 & 0x10; return v != 0; }
