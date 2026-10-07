#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 pad[0x1C]; u16 h1C; } S;
void func_800A8274(S *s) { s->h1C &= ~8; }
