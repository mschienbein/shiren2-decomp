#include "common.h"

typedef struct { char pad[0x74]; unsigned char x74; } S;
s32 func_800E32CC(S *s) { return s->x74 != 0; }
