#include "common.h"
typedef unsigned char u8;
extern s32 func_8010BEC4(void *obj, u8 id);
s32 func_8010EBFC(void *a){ s32 r = 0; if ((u8)func_8010BEC4(a, 0x37)) r = 1; else if ((u8)func_8010BEC4(a, 0x52)) r = 1; return r; }
