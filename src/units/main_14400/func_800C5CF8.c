#include "common.h"

typedef struct { char pad0[0x10]; s32 field_10; } S;
void func_800C5CF8(S *s, s32 v) { s->field_10 = v; }
