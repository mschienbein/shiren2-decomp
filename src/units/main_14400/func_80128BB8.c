#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad[0xF]; u8 f_F; } S;
void func_80128BB8(S *s, s32 v) { s->f_F = v; }
