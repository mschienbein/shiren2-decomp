#include "common.h"

extern void (*D_801CA710)(s32, s32);
void func_8012AAB0(void (*callback)(s32, s32)){D_801CA710=callback;}
