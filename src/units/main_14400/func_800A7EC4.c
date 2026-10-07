#include "common.h"

typedef struct { s32 a, b; } Tmp;
void *func_800A65E4(void *out_direction, void *obj, void *target);
void func_800A665C(void *p, Tmp *in);
void func_800A7EC4(void *p, void *target) { Tmp t; Tmp *tp = &t; func_800A65E4(tp, p, target); func_800A665C(p, tp); }
