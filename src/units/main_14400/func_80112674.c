#include "common.h"
extern s32 func_8010CB2C(void *, s32);
s32 func_80112674(void *p, s32 kind) {
    s32 r = 0;
    if (kind == 12 || func_8010CB2C(p, kind)) r = 1;
    return r;
}
