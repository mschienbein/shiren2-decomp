#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad[0x10]; u8 b10; } S;
void func_80128BC8(S *p, s32 v) { p->b10 = v; }
