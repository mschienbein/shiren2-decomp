#include "common.h"
/* Slot +0x44 receives two object pointers; this override ignores both. */
s32 func_800F8A54(void *arg0, void *arg1, unsigned char *out) {
    *out = 0;
    return 0;
}
