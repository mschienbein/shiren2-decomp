#include "common.h"

extern s32 func_800CD090(void *, void *);

/* The shared +0x64 predicate slot supplies arg; this base predicate ignores it. */
s32 func_800CE8F4(void *object, void *value, s32 arg) {
    return (u32)~func_800CD090(object, value) >> 31;
}
