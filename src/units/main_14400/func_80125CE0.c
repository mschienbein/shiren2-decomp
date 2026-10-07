#include "common.h"
typedef struct { s32 field0, field4; void *field8; } Object;
extern s32 D_80153AA0;
extern void func_800AC68C(Object *);
void func_80125CE0(Object *p, s32 flags) {
    p->field8 = &D_80153AA0;
    if (flags & 1) func_800AC68C(p);
}
