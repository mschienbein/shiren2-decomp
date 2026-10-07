#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 a, b; } Pair;
typedef struct { u8 pad[0x3C]; Pair x3C; } S;
u8 *func_8006A810(void *dst, s32 c, s32 n);
void func_8009D6E4(S *s, s32 flag) {
    Pair tmp;
    func_8006A810(&tmp, 0, 8);
    if (!flag) {
        tmp.a = 1;
    }
    s->x3C = tmp;
}
