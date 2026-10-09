#include "common.h"
extern void *func_800AC5B4(s32 size, s32 alternate);
extern void *func_80123330(void *obj);
/* The factory table D_8015797C must receive the constructed object. */
void *func_80123368(void) {
    return func_80123330(func_800AC5B4(0x10, 0));
}
