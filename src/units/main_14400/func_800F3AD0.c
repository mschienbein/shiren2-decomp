#include "common.h"
typedef struct { char pad[0x9A]; unsigned short f9A; } S9A;
s32 func_800F3AD0(S9A *p) { s32 r = p->f9A & 0x20; return r != 0; }
