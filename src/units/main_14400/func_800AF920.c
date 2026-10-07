#include "common.h"

extern unsigned char D_8015488C[];
typedef struct { s32 x0; unsigned char *x4; } S;
s32 func_800AF920(S *s, unsigned char n) { s32 i = n; return (s->x4[i >> 3] & D_8015488C[i & 7]) != 0; }
