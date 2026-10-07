#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 unk0; s32 unk4; s32 unk8; } S;
typedef struct { s32 x; s32 y; } Pos;
s32 func_80049CB4(s32, ...);
s32 func_800C4F18(S *arg0, Pos *from, Pos *to) { return func_80049CB4(arg0->unk8, from, to); }
