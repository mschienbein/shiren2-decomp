#include "common.h"

typedef unsigned short u16;
typedef struct { char pad[0x30]; u16 f30; } S;
u16 func_800E0ED0(S *s) { return s->f30; }
