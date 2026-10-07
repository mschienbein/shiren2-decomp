#include "common.h"
typedef struct { char pad[0x1C]; unsigned short x1C; } S;
void func_800A81B4(S *p) { p->x1C &= ~0x80; }
