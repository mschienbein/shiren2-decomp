#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern long func_80027290(void *);
extern long func_8002FEA0(void *, void **, long);
extern void func_80027310(void *);
extern u8 D_801E028C[], D_801D2C10[];
s32 func_80130DF0(void){ s32 r = func_80027290(D_801E028C); if (r) return r; func_8002FEA0(D_801E028C, 0, 1); func_80027310(D_801D2C10); return 0;}
