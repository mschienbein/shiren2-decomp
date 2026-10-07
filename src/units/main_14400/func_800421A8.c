#include "common.h"

s32 func_800610A8(void);
s32 func_801E8F54(void);
s32 func_800421A8(void) {
    if (func_800610A8() != 0) return func_801E8F54();
    return 0;
}
