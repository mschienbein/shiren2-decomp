#include "common.h"

typedef unsigned char u8;

/* Prefix view: +0x74 holds the unit pointer that func_80097070 passes to func_800A3D9C. */
typedef struct { u8 pad[0x74]; void *unit; } S;
extern u8 D_80138DE0[];
void func_80096BD0(S *s, void *p);
void func_8009702C(S *s, void *unit) { s->unit = unit; func_80096BD0(s, D_80138DE0); }
