#include "common.h"
typedef struct { short id; short pad; void *p; } S;
extern char D_80157FA8[], D_801582A8[];
S *func_800DA3E0(S *s){ s->p = D_80157FA8; s->id = 0x39; s->p = D_801582A8; return s; }
