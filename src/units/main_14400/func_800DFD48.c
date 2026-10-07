#include "common.h"

extern char D_80157FA8[];
typedef struct { short f0; short pad; void *f4; } S;
void func_800D8FE8(S*);
void func_800DFD48(S *a, s32 b){a->f4=D_80157FA8; if (b&1) func_800D8FE8(a);}
