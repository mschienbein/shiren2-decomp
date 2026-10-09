#include "common.h"
extern u32 func_80031F90(u32);
extern s32 D_801D2C0C;
void func_80131FF0(void) { u32 mask = func_80031F90(1); D_801D2C0C = 1; func_80031F90(mask); }
