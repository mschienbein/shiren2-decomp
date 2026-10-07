#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { u8 pad[0x2C]; u8 unk2C; } S;
u8 func_800A8C00(void *actor);
void func_801217B0(S *arg0, void *arg1) { arg0->unk2C = func_800A8C00(arg1); }
