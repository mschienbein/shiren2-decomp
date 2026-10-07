#include "common.h"
typedef struct { unsigned char pad[0x89]; unsigned char unk89; char pad8A[2]; void *unk8C; char pad90[0x18]; char unkA8[0x18]; unsigned char unkC0; } S;
s32 func_800CD278(void *container);
s32 func_800CD114(void *obj, s32 delta);
void func_80101750(S *s) { s32 d = s->unk89 - s->unkC0; if (d < 0) d = 0; func_800CD114(s->unkA8, d - func_800CD278(s->unk8C)); }
