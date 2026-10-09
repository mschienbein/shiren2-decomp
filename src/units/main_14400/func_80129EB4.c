#include "common.h"

typedef signed short s16;
extern s16 D_801CA6EC;
extern s16 D_801CA6EE;

void func_80129EB4(s32 flags, s32 value) {
    if (flags & 1) D_801CA6EC = value;
    if (flags & 2) D_801CA6EE = value;
}
