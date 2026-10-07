#include "common.h"

typedef struct { void *x0; s32 x4; s32 x8; void *xC; } S;
extern S D_80140130;
extern s32 D_80140134;
extern char D_80151350[], D_80149EE0[];
void func_800925AC(void) { S *p = &D_80140130; p->xC = D_80151350; D_80140134 = 0; p->xC = D_80149EE0; }
