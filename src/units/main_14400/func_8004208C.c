#include "common.h"

extern s32 func_800610A8(void);
extern s32 func_801F490C(s32, s32, s32);

s32 func_8004208C(s32 a, s32 b, s32 c) {
    s32 result;
    if (func_800610A8()) {
        result = func_801F490C(a, b, c);
    } else {
        result = 0;
    }
    return result;
}
