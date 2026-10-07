#include "common.h"

typedef struct { unsigned char a; unsigned char b; } Out;
typedef struct { unsigned char *f0; s32 f4; s32 f8; s32 fC; } Src;
void func_8005A048(s32 a, s32 b, s32 *c, s32 *d);
s32 func_80045D00(void);
s32 func_80045608(Src *s, s32 v, s32 w);
Out func_80045468(Src *s) {
    Out out;
    s32 v;
    s32 n;
    s32 d;
    func_8005A048(s->fC, s->f8, &v, &n);
    if (!func_80045D00()) {
        out.b = 0x80;
    } else {
        n = (unsigned char)(n + 1);
        out.b = n;
    }
    v >>= 5;
    v = (unsigned short)func_80045608(s, v, *s->f0);
    d = *s->f0 - v;
    if (d < 0) {
        d = 0;
    }
    out.a = d;
    return out;
}
