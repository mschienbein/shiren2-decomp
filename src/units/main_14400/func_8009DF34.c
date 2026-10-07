#include "common.h"
typedef struct { char pad[0x4C]; void *unk4C; } S;
extern char D_80151E38[];
void func_800D8FA8(void *object);
void func_8009DF34(S *s, s32 f) { s->unk4C = D_80151E38; if (f & 1) func_800D8FA8(s); }
