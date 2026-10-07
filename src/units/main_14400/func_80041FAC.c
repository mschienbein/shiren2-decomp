#include "common.h"

typedef unsigned char u8;

extern s32 func_800610A8(void);
extern u8 func_801E8CF4(u8 a, u8 b);

u8 func_80041FAC(u8 a, u8 b) {
    if (func_800610A8() == 0) {
        return 0;
    }
    return func_801E8CF4(a, b);
}
