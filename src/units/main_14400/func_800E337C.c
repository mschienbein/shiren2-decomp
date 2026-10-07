#include "common.h"

typedef struct { char pad[0x28]; unsigned short f28; } S;
s32 func_800E337C(S *s) { return s->f28; }
