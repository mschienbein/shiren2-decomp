#include "common.h"

typedef struct { char pad[0xCC]; char fCC; } S;
char *func_800EE260(S *s) { return &s->fCC; }
