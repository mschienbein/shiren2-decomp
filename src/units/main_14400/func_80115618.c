#include "common.h"
s32 func_8010CB2C(void *, s32);
s32 func_80115618(void *self, s32 arg) {
    s32 ret = 0;
    if (arg == 12 || func_8010CB2C(self, arg) != 0) {
        ret = 1;
    }
    return ret;
}
