#include "common.h"
typedef struct { s32 pad0; void *p4; s32 pad8; void *pC; char pad10[0x14]; s32 v24; char pad28[0xC]; s32 v34; char pad38[0xC]; void *p44; char pad48[0xC]; void *p54; } G;
extern G D_80140080;
extern char D_80151350[], D_80151400[], D_801513B0[], D_80151480[];
void func_80092038(void) {
    G *g = &D_80140080;
    g->pC = D_80151350;
    g->p4 = D_80151400;
    g->pC = D_801513B0;
    g->v24 = -1;
    g->v34 = -1;
    g->p44 = D_80151480;
    g->p54 = D_80151480;
}
