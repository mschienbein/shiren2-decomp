#include "common.h"

typedef unsigned char u8;
typedef struct { char pad[9]; u8 kind9; } S;
s32 func_800F3358(S *p);
s32 func_801054EC(S *p) { if ((p->kind9 & 0xF) != 1) return 1; return func_800F3358(p); }
