#include "common.h"
extern s32 func_800A4360(void *, void *);
extern s32 func_800A422C(void *, void *, s32);
s32 func_800A41EC(void *arg, void *value) { return func_800A422C(arg, value, func_800A4360(arg, value)); }
