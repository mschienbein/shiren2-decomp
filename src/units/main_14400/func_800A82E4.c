#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { u8 pad[0x1C]; u16 flags; } S;
void func_800A82E4(S *s) { s->flags |= 2; }
