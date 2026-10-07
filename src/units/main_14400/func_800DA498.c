#include "common.h"
typedef struct { unsigned short unk0; unsigned short pad2; void *unk4; } S;
extern char D_80157FA8[];
extern char D_801582D8[];
S *func_800DA498(S *s) { s->unk4 = D_80157FA8; s->unk0 = 0x3A; s->unk4 = D_801582D8; return s; }
