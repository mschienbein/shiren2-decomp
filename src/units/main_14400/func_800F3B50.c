#include "common.h"

typedef struct { char pad[0x9A]; unsigned short x9A; } S;
void func_800F3B50(S *s) { s->x9A |= 8; }
