#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 a; s32 b; } Key;
s32 func_800B4F74(Key *key);
s32 func_800B56F0(Key *key);
s32 func_80041660(s32 b, s32 a) {
    Key key;
    Key *p = &key;

    key.a = a;
    p->b = b;
    if (func_800B4F74(p) != 0) {
        return 1;
    }
    key.a = a;
    p->b = b;
    return func_800B56F0(p);
}
