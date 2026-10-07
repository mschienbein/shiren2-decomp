#include "common.h"
typedef unsigned short u16;
typedef struct { char pad[0xE4]; u16 flags; } S;
void func_800EE2E4(S *p) { p->flags |= 8; }
