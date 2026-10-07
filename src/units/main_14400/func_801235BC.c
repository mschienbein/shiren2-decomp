#include "common.h"

extern s32 D_80153AA0;
typedef struct { char pad[8]; s32 *vt8; } S;
void func_800AC68C(S *p);
void func_801235BC(S *p, s32 flags) {
    p->vt8 = &D_80153AA0;
    if (flags & 1) func_800AC68C(p);
}
