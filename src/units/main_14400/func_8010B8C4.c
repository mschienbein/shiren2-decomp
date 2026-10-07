#include "common.h"

typedef struct { char pad[0xCC]; s32 fCC; } S;
s32 *func_8010B8C4(S *s) { return &s->fCC; }
