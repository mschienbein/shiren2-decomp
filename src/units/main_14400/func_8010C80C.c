#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

s32 func_8010BD00(void *arg0, u8 arg1);
s32 func_8010C80C(void *arg0) {
    return func_8010BD00(arg0, 0x1D);
}
