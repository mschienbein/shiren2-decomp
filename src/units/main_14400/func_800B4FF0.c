#include "common.h"
void *func_800B4D80(void *pos);
s32 func_800B4FF0(void *pos, s32 kind) {
    unsigned char *entry = func_800B4D80(pos);
    if (entry != 0 && *entry == kind) return 1;
    return 0;
}
