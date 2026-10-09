#include "common.h"

typedef unsigned char u8;

typedef struct {
    signed char x;
    signed char y;
} Pair800459A8;

typedef struct Buf800459A8 Buf800459A8;

typedef struct {
    s32 a;
    s32 b;
} Key800453E4;

extern s32 func_80045D00(void);
extern s32 func_8004552C(Buf800459A8 *object, s32 value);
extern s32 func_8004559C(Buf800459A8 *s, Key800453E4 *key);

Pair800459A8 func_800453E4(Buf800459A8 *buf, Key800453E4 *key)
{
    Pair800459A8 result;

    if (func_80045D00() == 1) {
        result.y = func_8004552C(buf, key->b);
    } else {
        *(u8 *)&result.y = 0x80;
    }
    result.x = func_8004559C(buf, key);
    return result;
}
