#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x0; void *x4; } S;
extern u8 D_80149DA8[];
extern u8 D_80149DB8[];
void func_800D8FA8(void *object);
void func_8013680C(S *s, s32 flags) {
    s->x4 = D_80149DA8;
    s->x0 = 0;
    s->x4 = D_80149DB8;
    if (flags & 1) {
        func_800D8FA8(s);
    }
}
