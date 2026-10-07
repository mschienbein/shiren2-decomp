#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

extern u8 D_80156B88[];
u8 func_800A9A78(u8 index) {
    return D_80156B88[index];
}
