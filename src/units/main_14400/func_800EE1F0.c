#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad[0x94]; u8 b94; } S;
void func_800EE1F0(S *s) { s->b94 |= 1; }
