#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { u8 pad[4]; s16 state; u8 pad6[0xE]; s32 handle; u8 pad18[0x10]; s32 x28; s32 x2C; u8 pad30[0x2C]; s32 x5C; u8 pad60[8]; s32 x68; } S;
s32 func_800748F8(s32 owner, s32 type, s32 x, s32 z, s32 direction, s32 variant);
void func_800887A4(S *s) { func_800748F8(s->handle, 0, s->x5C, s->x68, s->x2C, s->x28); s->state = 4; }
