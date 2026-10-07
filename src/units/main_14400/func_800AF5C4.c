#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad[2]; u8 flags2; } S;
void func_800AF5C4(S *p) { p->flags2 &= ~2; }
