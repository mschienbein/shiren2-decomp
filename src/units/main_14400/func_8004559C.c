#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 a; s32 b; } Pair;
typedef struct { u8 *f_0; u8 pad[0xC]; Pair f_10; } S;
s32 func_800A23E8(Pair *p, Pair *key);
s32 func_80045608(S *s, s32 v, s32 c);
s32 func_8004559C(S *s, Pair *key) {
    Pair k;
    Pair *kp = &k;
    s32 r, d;
    kp->a = key->a;
    kp->b = key->b;
    r = (u16)func_80045608(s, func_800A23E8(&s->f_10, kp), *s->f_0);
    d = *s->f_0 - r;
    if (d < 0) d = 0;
    return d;
}
