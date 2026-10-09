#include "common.h"
typedef unsigned char u8;
typedef struct S S;
extern s32 func_8010BEC4(S *s, u8 c);
s32 func_801114D8(S *obj) { return (u8)func_8010BEC4(obj, 0x4E) != 0; }
