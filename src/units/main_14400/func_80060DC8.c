#include "common.h"
extern s32 func_80054D50(void); /* status result intentionally discarded */
extern void func_80054DE0(s32);
extern unsigned char func_8006C508(unsigned char); /* previous byte intentionally discarded */
void func_80060DC8(void) { func_80054D50(); func_80054DE0(1); func_8006C508(1); }
