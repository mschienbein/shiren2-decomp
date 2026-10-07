#include "common.h"

void *func_800AC5B4(s32 a, s32 b);
void *func_801187D0(void *a);
void *func_80118808(void) {
    return func_801187D0(func_800AC5B4(12, 0));
}
