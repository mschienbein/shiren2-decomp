#include "common.h"
s32 func_80081C18(s32 a, s32 b, s32 c, s32 d, void (*callback)(void *), void *arg);
s32 func_80081D60(s32 a, s32 b, s32 c, s32 d, void (*callback)(void *)) { return func_80081C18(a, b, c, d, callback, 0); }
