#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 a, b; } Pair;
void func_800B6590(void *self, Pair *p, Pair *q);
void func_800B6728(void *self, s32 *src) {
    Pair p, q;
    p.a = src[0];
    p.b = src[1];
    q.a = src[2];
    q.b = src[3];
    func_800B6590(self, &p, &q);
}
