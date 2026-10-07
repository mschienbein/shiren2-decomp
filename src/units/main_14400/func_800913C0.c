#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

extern void *D_80140070;
extern void *func_800913F4(void *buffer, u32 size);

s32 func_800913C0(void *buffer, u32 size) {
    D_80140070 = func_800913F4(buffer, size);
    if (D_80140070 == 0) {
        return -1;
    }
    return 0;
}
