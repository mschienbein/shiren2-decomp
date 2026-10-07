#include "common.h"
typedef struct { char pad[8]; void *f8; } SA;
extern char D_80153AA0[];
void func_800AC68C(SA *p);
void func_801247F0(SA *p, s32 flags) {
    p->f8 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(p);
    }
}
