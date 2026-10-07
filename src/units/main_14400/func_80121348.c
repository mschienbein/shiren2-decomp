#include "common.h"
typedef struct { s32 x0; s32 x4; void *x8; } S;
extern char D_8015F898[];
void *func_80114060(void *obj, s32 kind);
void func_80121388(S *p);
S *func_80121348(S *p) {
    func_80114060(p, 0xAB);
    p->x8 = D_8015F898;
    func_80121388(p);
    return p;
}
