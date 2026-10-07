#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

extern u8 D_801398B8;
extern u16 D_80162B46;
extern u16 D_80162B48;

s32 func_80054B10(void) {
    if (D_801398B8 == 1) {
        return 2;
    }
    return D_80162B46 != D_80162B48;
}
