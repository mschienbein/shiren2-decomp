#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad[0xE4]; u16 f_E4; } S;
void func_800EE324(S *s) { s->f_E4 |= 2; }
