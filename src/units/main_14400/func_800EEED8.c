#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad[0xBA]; u8 f_BA; u8 f_BB; u8 f_BC; } S;
void func_800EA7D4(S *s);
void func_800EEED8(S *s) { func_800EA7D4(s); s->f_BA = 0; s->f_BB = 0; s->f_BC = 0; }
