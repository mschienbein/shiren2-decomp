#include "common.h"
extern s32 func_800B5230(void *object);
s32 func_800B52D0(void *object) {
    u32 value = func_800B5230(object) & 0xFF;
    if (value >= 250U) {
        return value - 249;
    }
    return 0;
}
