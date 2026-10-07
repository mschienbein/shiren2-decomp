#include "common.h"

typedef struct { short f0; short pad; void *f4; s32 f8; } S;
extern char D_80154668[], D_80154300[];
void func_800D0E38(S*); void func_800D8FA8(void *obj);
void func_800D0690(S *a, s32 b){ a->f4 = D_80154668; a->f8 = 1; func_800D0E38(a); a->f4 = D_80154300; if (b & 1) func_800D8FA8(a); }
