#include "common.h"

typedef struct { void *x0; s32 x4; u32 x8; } S;
void *func_800A09B0(S *s, void *buf, u32 size) { s->x0 = buf; s->x4 = 0; s->x8 = size; return s; }
