#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad[0xD4]; u8 bD4; } S;
u8 *func_80129B5C(S *s, u8 *p) { s->bD4 = *p++; return p; }
