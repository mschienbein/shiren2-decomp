#include "common.h"

typedef struct { char pad[0x8A]; unsigned char f8A; } S;
s32 func_80100D3C(S *s) { return s->f8A; }
