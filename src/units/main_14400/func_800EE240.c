#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x10A]; u8 field_10A; } S;
u8 func_800EE240(S *s) { return s->field_10A; }
