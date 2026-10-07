#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern u8 D_80142920[];
s32 func_800A1630(void *arg0, u8 arg1);
s32 func_80042AB0(s32 id) {
    return func_800A1630(D_80142920, (u8)id);
}
