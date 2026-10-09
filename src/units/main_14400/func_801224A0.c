#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

extern u8 D_80148780[3][3];
extern void func_801224E8(unsigned char *arg0);

void func_801224A0(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        func_801224E8(D_80148780[i]);
    }
}
