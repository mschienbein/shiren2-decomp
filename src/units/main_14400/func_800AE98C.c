#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

extern u8 D_8015374C[];

u8 func_800AE98C(u8 *id) {
    return id[1] - D_8015374C[id[0]];
}
