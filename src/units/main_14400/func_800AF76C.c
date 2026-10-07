#include "common.h"

typedef struct { char pad[2]; unsigned char x2; } S;
s32 func_800AF76C(S *s, s32 mask) { return (s->x2 & mask) != 0; }
