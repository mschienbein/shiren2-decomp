#include "common.h"

typedef short s16;
typedef struct { char pad[0x2A]; s16 f2A; } S;
void func_800E3374(S *s, s16 v) { s->f2A = v; }
