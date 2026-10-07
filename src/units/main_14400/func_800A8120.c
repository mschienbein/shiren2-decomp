#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad[0x1C]; u8 b1C; } S;
u32 func_800A8120(S *s) { return s->b1C >> 7; }
