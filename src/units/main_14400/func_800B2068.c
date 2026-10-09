#include "common.h"

extern s32 func_800B200C(void *);
s32 func_800B2068(void *first, void *second) { s32 key = func_800B200C(first); s32 result = 0; if (key >= 0) result = key == func_800B200C(second); return result; }
