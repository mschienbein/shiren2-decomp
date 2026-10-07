#include "common.h"

typedef struct { char pad[0x10]; s32 unk10; } S;
void func_801128A8(S *s, s32 v) { s->unk10 = v; }
