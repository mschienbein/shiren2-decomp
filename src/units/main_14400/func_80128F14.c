#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad[0xC]; u8 xC; } S;
void func_80128F14(S *s, u8 v) { s->xC = v; }
