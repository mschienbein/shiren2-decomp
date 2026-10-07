#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

extern s32 D_801CA700;
extern s32 D_801CA704;
s32 func_8012A898(s32 value);

void func_8012A8C8(s32 value) {
    s32 ok = 1;

    if (value == 0) {
        ok = func_8012A898(D_801CA704);
    }
    if (ok) {
        D_801CA700 = value;
    }
}
