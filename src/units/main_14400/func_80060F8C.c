#include "common.h"

extern void func_8006E7E0(void);
extern s32 func_80056E64(s32 mode); /* initialization status intentionally discarded */
extern unsigned char func_8006C508(unsigned char value); /* previous byte intentionally discarded */
void func_80060F8C(void) { func_8006E7E0(); func_80056E64(1); func_8006C508(2); }
