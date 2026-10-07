#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern s32 func_800B5690(void *);
extern s32 func_80049CB4(s32 id, ...);
s32 func_800B5650(void *p){ if (func_800B5690(p)) { func_80049CB4(0xD7, p); return 1; } return 0;}
