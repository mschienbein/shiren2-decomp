#include "common.h"

extern s32 D_80158508;
typedef struct { s32 x0; s32 *vt4; } S;
void *func_800DA8A0(void *obj, s32 kind, void *src);
S *func_800DB9A0(S *p, void *src) {
    func_800DA8A0(p, 0x13, src);
    p->vt4 = &D_80158508;
    return p;
}
