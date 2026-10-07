#include "common.h"

typedef struct { s32 unk0; void *unk4; } S;
extern char D_80157FA8[];
extern void func_800D8FE8(S *);
void func_800DCC88(S *s, s32 flag) {
    s->unk4 = D_80157FA8;
    if (flag & 1) func_800D8FE8(s);
}
