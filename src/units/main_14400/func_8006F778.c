#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern s32 func_8006F958(u8 *);
extern s32 func_80070380(u8 *);
extern s32 func_80070590(void);
s32 func_8006F778(u8 *p){
    s32 r = 11;
    switch (p[0]) {
    case 0: r = func_8006F958(p) + 11; break;
    case 1: r = func_80070380(p) + 11; break;
    case 2: r = func_80070590() + 11; break;
    }
    return r;
}
