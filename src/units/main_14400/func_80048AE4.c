#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

s32 func_800A99D0(void);
s32 func_80048AE4(void) { return func_800A99D0() ^ 1; }
