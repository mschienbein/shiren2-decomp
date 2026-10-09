#include "common.h"
typedef unsigned char u8;
typedef struct S S;
extern s32 func_8010BEC4(S *s, u8 c);
s32 func_8010DD14(S *object) { return (u8)func_8010BEC4(object, 0x6A) != 0; }
