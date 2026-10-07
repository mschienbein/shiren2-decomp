#include "common.h"

typedef struct { char pad0[0xB0]; char field_B0[1]; } S;
char *func_800DDD44(S *s) { return s->field_B0; }
