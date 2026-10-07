#include "common.h"
extern s32 func_8010BEC4(void *, unsigned char);
s32 func_8010BF0C(void *arg) { s32 result = 0; if ((unsigned char)func_8010BEC4(arg, 0x69) || (unsigned char)func_8010BEC4(arg, 0x22)) result = 1; return result; }
