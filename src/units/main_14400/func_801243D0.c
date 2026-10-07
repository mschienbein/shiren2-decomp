#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad[8]; void *x8; } S;
extern u8 D_80153AA0[];
void func_800AC68C(void *p);
void func_801243D0(S *s, s32 flags) {
    s->x8 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(s);
    }
}
