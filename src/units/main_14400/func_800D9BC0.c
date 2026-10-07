#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
typedef struct { u8 x[0x18]; } T; extern T D_80143330[];
s32 func_800D9BC0(u8 *a, u8 *out){ s32 i; T *p; *out++ = a[1]; p = *(T**)(a+8); for (i=0;i<2;i++){ if (p==&D_80143330[i]) break; } *out = i; return 2; }
