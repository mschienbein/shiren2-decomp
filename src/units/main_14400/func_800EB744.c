#include "common.h"

typedef struct { char pad0[0x84]; s32 field_84; } S;
void func_800EB744(S *s, s32 delta) {
    s32 v = s->field_84 + delta;
    if (v < 0) {
        v = 0;
    } else if ((u32)v > 999999) {
        v = 999999;
    }
    s->field_84 = v;
}
