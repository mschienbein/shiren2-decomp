#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x89]; u8 field_89; } S;
u8 func_800FF3E4(S *s) { return s->field_89; }
