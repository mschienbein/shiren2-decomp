#include "common.h"

typedef unsigned char u8;
typedef struct S S;

s32 func_8010BEC4(S *s, u8 c);

s32 func_801114B4(S *s) {
    return (u8)func_8010BEC4(s, 0x78) != 0;
}
