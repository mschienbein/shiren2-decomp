#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x81]; u8 field_81; } S;
u8 func_800F3CD8(S *s) { return s->field_81; }
