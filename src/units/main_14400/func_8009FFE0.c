#include "common.h"

typedef short s16;
typedef unsigned short u16;
u32 func_8009FF80(u16 a, u16 b);
s32 func_8009FFE0(u16 a, u16 b) {
    if ((s16)a > 0) {
        return (s32)func_8009FF80(a, b) >> 16;
    }
    return (s16)a;
}
