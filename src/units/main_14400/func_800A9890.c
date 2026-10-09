#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern char *func_800A9720(u8 kind, u8 variant, u8 flags);
char *func_800A9890(u8 a, u8 b){ return func_800A9720(a, b, 3);}
