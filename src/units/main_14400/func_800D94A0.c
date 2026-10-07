#include "common.h"

typedef struct { short f0; short pad; void *f4; } S;
extern char D_80157FA8[], D_80158038[];
S *func_800D94A0(S *a){a->f4=D_80157FA8; a->f0=7; a->f4=D_80158038; return a;}
