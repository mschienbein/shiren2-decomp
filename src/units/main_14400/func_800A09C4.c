#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

void func_800A09C4(void *stream, void *buf, u32 size) { u8 *p = buf; while (size-- != 0) p[size] = 0; }
