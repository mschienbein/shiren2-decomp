#include "common.h"
extern void *func_800A7DEC(void *obj);
extern s32 func_800A46BC(void *obj, void *pos, void *dir);
s32 func_800A800C(void *arg, void *dir) { return func_800A46BC(arg, func_800A7DEC(arg), dir); }
