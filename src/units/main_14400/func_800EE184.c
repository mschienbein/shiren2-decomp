#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { u8 pad[0x94]; u8 unk94; } S;
void func_800EE184(S *arg0) { arg0->unk94 &= ~4; }
