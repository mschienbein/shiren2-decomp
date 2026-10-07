#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[4]; u8 field_4; } S;
void func_800AF610(S *s, u8 v) { s->field_4 = v; }
