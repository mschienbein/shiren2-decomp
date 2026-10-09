#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

/* func_80068918 never sets v0 (its seed < 0 path returns straight after two calls). */
void func_80068918(s32 a);
/* self: the caller (func_800BA854) passes its object in a0; unused here. */
void func_800435A8(void *self, s32 b) { func_80068918(b); }
