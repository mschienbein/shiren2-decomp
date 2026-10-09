#include "common.h"
extern s32 func_800A42CC(void *, void *, s32);
extern s32 func_800B4F74(void *);
s32 func_800F4A28(void *first, void *second, s32 mode) {
    s32 result = 0;
    if (func_800A42CC(first, second, mode)) result = func_800B4F74(second) == 0;
    return result;
}
