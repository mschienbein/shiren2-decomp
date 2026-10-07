#include "common.h"

typedef short s16;
typedef struct { char pad0[0x2E]; s16 field_2E; } S;
void func_800E335C(S *s, s16 v) { s->field_2E = v; }
