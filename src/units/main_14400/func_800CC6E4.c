#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad[4]; u8 f_4; } S;
s32 func_800CC6E4(S *s) { return s->f_4 == 0; }
