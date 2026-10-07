#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern s32 func_8008C6B8(u8*);
s32 func_8008D1B0(u8 *a){ u32 i; u8 *p = a; for (i=0;i<8;i++, p+=0x20){ if (*p != 1) { if (func_8008C6B8(p)) return -1; (*(s32*)(a+0x4C0))++; return i; } } return -1; }
