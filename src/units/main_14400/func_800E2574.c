#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x76]; u8 field_76; } S;
u8 func_800E2574(S *s) { return s->field_76; }
