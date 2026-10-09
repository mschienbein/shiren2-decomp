#include "common.h"
extern u32 D_801D2C0C;
u32 func_80031F90(u32 mask);
u32 func_80132020(void) { u32 mask = func_80031F90(1); D_801D2C0C = 0; return func_80031F90(mask); }
