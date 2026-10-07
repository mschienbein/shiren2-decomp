#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern s32 D_8016FD50;
extern s32 D_8016FD54;
void func_8006A1C8(s32 x, s32 y) {
    if (x >= 0) {
        if (x <= 1000) {
            D_8016FD50 = x;
        } else {
            D_8016FD50 = 1000;
        }
    } else {
        D_8016FD50 = 0;
    }
    if (y >= 0) {
        if (y <= 1000) {
            D_8016FD54 = y;
        } else {
            D_8016FD54 = 1000;
        }
    } else {
        D_8016FD54 = 0;
    }
}
