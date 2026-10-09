#include "common.h"
typedef struct { s32 x0; s32 x4; void *x8; unsigned char xC; unsigned char xD; } S;
extern char D_8015D888[];
void *func_800AC0C0(S *p, s32 kind, s32 arg);
S *func_80113520(S *p, s32 arg) {
    func_800AC0C0(p, 6, arg);
    p->x8 = D_8015D888;
    p->xC = 0;
    p->xD = 0;
    return p;
}
