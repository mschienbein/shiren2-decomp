#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

extern u8 func_800C57A0(void *arg);

s32 func_800C5B20(void *arg, u16 alignment) {
    return (func_800C57A0(arg) & (alignment - 1)) == 0;
}
