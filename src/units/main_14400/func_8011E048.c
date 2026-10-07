#include "common.h"

typedef struct { s32 unk0; s32 unk4; void *unk8; } S;
extern char D_80153AA0[];
extern void func_800AC68C(S *);
void func_8011E048(S *s, s32 flag) {
    s->unk8 = D_80153AA0;
    if (flag & 1) func_800AC68C(s);
}
