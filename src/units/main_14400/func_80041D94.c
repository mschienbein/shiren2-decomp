#include "common.h"

typedef unsigned short u16;

extern u16 D_8014767C;

s32 func_80041D94(void) {
    return (D_8014767C & 3) != 0;
}
