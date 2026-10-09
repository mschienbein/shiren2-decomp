#include "common.h"

/* Slot +0x44 receives two object pointers; this override ignores both. */
s32 func_800F8BD0(void *arg0, void *arg1, unsigned char *output) {
    *output = 0;
    return 0;
}
