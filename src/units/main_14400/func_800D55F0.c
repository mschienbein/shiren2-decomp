#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

void *func_800D4A60(void *owner, s32 key);
void *func_800D55F0(void *arg0, u32 arg1) {
    if (arg1 < 3) {
        return func_800D4A60(arg0, arg1);
    }
    return 0;
}
