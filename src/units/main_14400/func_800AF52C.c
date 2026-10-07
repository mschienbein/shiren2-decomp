#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 a; s32 b; } Pair;
s32 func_800ADF20(void *obj, void *avoid);
void func_800AF52C(void *obj, Pair *src) {
    Pair copy;
    Pair *p = &copy;

    copy.a = src->a;
    p->b = src->b;
    func_800ADF20(obj, p);
}
