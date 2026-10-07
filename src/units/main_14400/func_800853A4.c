#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { u8 pad[4]; s16 state; u8 pad6[0xE]; s32 handle; } S;
void func_80074084(s32);
void func_800853A4(S *s) { func_80074084(s->handle); s->state = 4; }
