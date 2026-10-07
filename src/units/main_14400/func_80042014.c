#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern u16 D_801F5D0E;
extern u16 D_801F5D10;
s32 func_800610A8(void);
s32 func_80042014(void) {
    if (func_800610A8() == 0) {
        return 0;
    }
    return D_801F5D0E != 0 || D_801F5D10 != 0;
}
