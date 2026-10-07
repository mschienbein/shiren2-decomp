#include "common.h"
s32 func_8010B9CC(void *unused, unsigned char a, unsigned char b) {
    s32 v = a + b - 10;
    s32 r = 0xFF;
    if (v < 0x100) r = v;
    return r;
}
