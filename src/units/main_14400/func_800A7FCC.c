#include "common.h"
extern void *func_800A7DEC(void *obj);
extern s32 func_800A4CC4(void *obj, void *pos, void *dir);
s32 func_800A7FCC(void *p, void *dir) { return func_800A4CC4(p, func_800A7DEC(p), dir); }
