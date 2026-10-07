#include "common.h"
typedef struct { s32 x, y; } Pair;
typedef struct { char pad[0x84]; s32 x; s32 y; } S;
Pair *func_800F6B40(Pair *r, S *p) { r->x = p->x; r->y = p->y; return r; }
