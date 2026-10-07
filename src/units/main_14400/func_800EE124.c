#include "common.h"
typedef struct { char pad[0xE4]; unsigned short xE4; } S;
void func_800EE124(S *p) { p->xE4 &= ~0x40; }
