#include "common.h"

typedef struct { char pad[0x88]; s32 x88; } S;
s32 func_800EB37C(S *s) { return (s->x88 + 999) / 1000; }
