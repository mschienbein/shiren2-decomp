#include "common.h"
typedef struct { s32 x0; void *x4; } S;
extern char D_80157FA8[];
void func_800D8FE8(S *p);
void func_800DCAC4(S *p, s32 flags) {
    p->x4 = D_80157FA8;
    if (flags & 1) func_800D8FE8(p);
}
