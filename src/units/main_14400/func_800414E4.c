#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
typedef struct { s32 x0; s32 x4; } Arg; extern u32 func_800B1C6C(Arg*);
s32 func_800414E4(s32 a, s32 b){ Arg s; Arg *p=&s; s32 r=0; s.x4=a; s.x0=b; if ((func_800B1C6C(p)&0x4000) || (func_800B1C6C(p)&0x8000)) r=1; return r; }
