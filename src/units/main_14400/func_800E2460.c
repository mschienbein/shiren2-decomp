#include "common.h"
typedef struct { unsigned char pad[0x55]; unsigned char unk55; } S;
s32 func_800E2460(S *s) { return s->unk55 >> 4; }
