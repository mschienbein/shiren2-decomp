#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
typedef struct { u8 x[0x18]; } B;
extern void *func_800C4F34(B*, u16); extern s32 func_800A6EE0(void*); extern void *func_800C4DA0(B*, void*, B*, u16); extern void func_800C4864(B*, s32, void*);
void func_800A56D8(void *a, void *b, s16 c, u16 d){ B b1; B b2; func_800C4F34(&b1, (u16)c); d |= func_800A6EE0(a); func_800C4DA0(&b2, a, &b1, d); func_800C4864(&b2, 0x38, b); }
