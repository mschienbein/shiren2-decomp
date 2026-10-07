#include "common.h"
typedef struct { char pad[2]; unsigned char f2; } S2;
void func_800AF6E0(S2 *p) { p->f2 |= 0x20; }
