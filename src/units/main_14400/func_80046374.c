#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { unsigned char color[4], state[0x20]; float values[8]; unsigned char active, pad45[3]; } SavedState;
extern s32 D_80138BF0;
extern SavedState D_80138BF4;
s32 func_800A99D0(void);
void func_8005C59C(void *a, void *b, void *c, void *d);
void func_801E583C(s32 arg);
void func_80046374(void) {
    if (D_80138BF0 == 0 && func_800A99D0() != 0) {
        func_8005C59C(D_80138BF4.values, &D_80138BF4.active, D_80138BF4.state, D_80138BF4.color);
        D_80138BF0 = 1;
        func_801E583C(0);
        return;
    }
    if (D_80138BF0 == 0) {
        D_80138BF0 = 1;
    }
}
