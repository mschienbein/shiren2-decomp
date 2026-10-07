#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

s32 func_800A3138(void *p);
s32 func_800A315C(void *p);
s32 func_800A3214(void *p) { return func_800A3138(p) * func_800A315C(p); }
