#include "common.h"
extern s32 D_8013CA20;
extern void func_8006AAF0(void *dst, u32 devAddr, s32 size);
/* romBase is a PI ROM address and segAddr a segmented address; neither is dereferenced. */
void func_8006AC30(void *dst, void *romBase, void *segAddr, s32 stride, s32 first, s32 count) { u32 devAddr; if(stride&1) stride++; devAddr=(u32)romBase+((u32)segAddr&0xFFFFFF)+stride*first; if(D_8013CA20) func_8006AAF0(dst,devAddr,stride*count); }
