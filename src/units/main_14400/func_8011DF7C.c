#include "common.h"
typedef struct { s32 x0; s32 x4; void *x8; } S;
extern char D_80153AA0[];
void func_800AC68C(S *p);
void func_8011DF7C(S *p, s32 flags) {
    p->x8 = D_80153AA0;
    if (flags & 1) func_800AC68C(p);
}
