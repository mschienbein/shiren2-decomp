#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern s32 D_8013FEE0;
void *D_8013FEE4 = 0;
extern s32 D_8013FEE8;
extern u8 D_801C33D0[];
void func_8006CD14(void);
void func_8008CF78(void *bank);
void func_8008E318(void *ctx);
s32 func_8008E264(void *ctx, s32 value);
s32 func_8008C384(s32 value) {
    s32 old = D_8013FEE8;
    D_8013FEE8 = value;
    if (D_8013FEE0 != 0) {
        func_8006CD14();
        func_8008CF78(D_8013FEE4);
        func_8008E318(D_801C33D0);
        func_8008E264(D_801C33D0, D_8013FEE8);
    }
    return old;
}
