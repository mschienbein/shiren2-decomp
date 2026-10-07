#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

void func_8010DDF8(u8 *p, s16 v) { *(s16 *)(p + 0xC) = v; }
