#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad[0x9A]; u16 x9A; } S;
void func_800F3AC0(S *s) { s->x9A |= 0x80; }
