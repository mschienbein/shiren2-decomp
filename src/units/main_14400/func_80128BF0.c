#include "common.h"
typedef struct { char pad[0xC]; unsigned char xC; } S;
s32 func_80128BF0(S *p, s32 mask) { return (p->xC & mask) != 0; }
