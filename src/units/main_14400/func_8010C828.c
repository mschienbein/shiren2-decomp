#include "common.h"

typedef unsigned char u8;
typedef struct S8010BEC4 S8010BEC4;

extern s32 func_8010BEC4(S8010BEC4 *s, u8 c);

s32 func_8010C828(S8010BEC4 *s, u8 c) {
    return (u8)func_8010BEC4(s, c) != 0;
}
