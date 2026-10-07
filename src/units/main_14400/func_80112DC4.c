#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern u16 D_80157AE8[];
u16 func_80112DC4(u8 id) {
    return D_80157AE8[id - 0x17];
}
