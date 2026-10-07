#include "common.h"
extern unsigned char D_80153AA0[];
extern void func_800AC68C(void *);
void func_80123E58(void **arg, s32 flags) { arg[2] = D_80153AA0; if (flags & 1) func_800AC68C(arg); }
