#include "common.h"

extern char D_801583B8[];
typedef struct { short f0; short pad; void *f4; } S;
void *func_800DA904(void *obj, s32 kind, unsigned char *params);
S *func_800DAE98(S *a, unsigned char *b){func_800DA904(a, 12, b); a->f4=D_801583B8; return a;}
