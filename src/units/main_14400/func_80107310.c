#include "common.h"

typedef struct { char pad[0xA0]; s32 fA0; } S;
void func_80107310(S *s, s32 v) { s->fA0 = v; }
