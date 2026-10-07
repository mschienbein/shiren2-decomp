#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad[0x52]; u8 x52; } S;
void func_800E23B8(S *s) { s->x52 = 1; }
