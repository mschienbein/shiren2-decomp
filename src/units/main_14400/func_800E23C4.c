#include "common.h"
typedef struct { char pad[0x54]; unsigned char f54; } S54;
s32 func_800E23C4(S54 *p) { return p->f54 & 1; }
