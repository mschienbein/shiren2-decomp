#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad[2]; u8 x2; } S;
void func_800AF6D0(S *s) { s->x2 &= ~0x20; }
