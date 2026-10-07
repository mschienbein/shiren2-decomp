#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 a, b; } Pair;
typedef struct { u8 pad[0x34]; Pair x34; u8 pad2[0x54 - 0x3C]; s32 x54; } S;
void func_800488F0(S *s, Pair *p, s32 v, Pair *dst);
void func_80097418(S *s, Pair *p) {
    func_800488F0(s, p, s->x54, &s->x34);
    s->x34 = *p;
}
