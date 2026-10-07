#include "common.h"
extern unsigned char D_80165974;
extern void func_8005CE44(s32);
void func_8005DB20(void) { unsigned char *p = &D_80165974; if (*p) { *p = 0; func_8005CE44(0); func_8005CE44(1); } }
