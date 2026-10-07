#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { u8 pad[0x43]; u8 x43[5][4]; } S;
s32 func_800D1604(S *s, s32 row){ s32 n = 0; s32 i; for (i = 0; i < 4; i++) { if (s->x43[row][i]) n++; } return n;}
