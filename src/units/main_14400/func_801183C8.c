#include "common.h"

extern void *func_800AC5B4(s32 size, s32 arg1);
extern void *func_80118390(void *obj);

void *func_801183C8(void) {
    return func_80118390(func_800AC5B4(0xC, 0));
}
