#include "common.h"

typedef struct { char pad[0x9E]; unsigned char f9E; } S;
void func_800F3A00(S *s, s32 v) { s->f9E = v; }
