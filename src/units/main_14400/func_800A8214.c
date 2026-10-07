#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { u8 pad[0x1C]; u16 unk1C; } S;
void func_800A8214(S *arg0) { arg0->unk1C &= ~0x20; }
