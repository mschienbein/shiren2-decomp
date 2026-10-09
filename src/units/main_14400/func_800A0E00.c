#include "common.h"
extern s32 func_800CE46C(void *list, s32 (*cb)(void *));
s32 func_800A0E00(void **list, s32 (*cb)(void *)) { return func_800CE46C(*list, cb); }
