#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { s32 a, b; } Pair;
typedef struct { u8 pad[0x8C]; s32 a; s32 b; } S;
Pair *func_800F6B28(Pair *out, S *s) { out->a = s->a; out->b = s->b; return out; }
