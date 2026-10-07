#include "common.h"

typedef short s16;
typedef unsigned char u8;

extern s32 func_800E04D0(void *obj);
extern s16 D_80158C6C[4];

s32 func_800E2478(void *obj) {
    return D_80158C6C[(u8)func_800E04D0(obj)];
}
