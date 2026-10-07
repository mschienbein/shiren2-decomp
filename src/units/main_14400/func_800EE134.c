#include "common.h"

typedef struct { char pad[0xE4]; unsigned short unkE4; } S;
s32 func_800EE134(S *s) { return (s->unkE4 >> 6) & 1; }
