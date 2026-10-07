#include "common.h"

typedef unsigned short u16;
typedef struct { char pad[0x9A]; u16 flags9A; } S;
s32 func_800F3A18(S *p) { if (p->flags9A & 0x200) return 1; return 0; }