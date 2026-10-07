#include "common.h"
typedef unsigned short u16;
typedef struct { char pad[0x1C]; u16 flags; } S;
void func_800A8300(S *p) { p->flags &= ~1; }
