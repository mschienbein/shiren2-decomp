#include "common.h"
typedef struct { s32 unk0; void *unk4; } S;
extern char D_801586B8[];
void *func_800DA8A0(void *obj, s32 kind, void *src);
S *func_800DCB00(S *s, void *src) { func_800DA8A0(s, 0x1C, src); s->unk4 = D_801586B8; return s; }
