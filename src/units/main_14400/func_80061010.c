#include "common.h"
extern char D_801E4E48[];
extern void func_800612D4(s32);
extern unsigned char func_8006C508(unsigned char value);
extern void func_8006E7E0(void),func_800553B0(void),func_80074374(void),func_800725E0(void);
extern s32 func_8006E908(void *pool, s32 count1, s32 count2, s32 count3);
void func_80061010(void) { func_800612D4(0); func_8006E7E0(); func_800553B0(); func_80074374(); func_800725E0(); func_8006E908(D_801E4E48,0x1900,0x1100,0x180); func_8006C508(1); }
