#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 unk0; s32 unk4; s32 unk8; void *unkC; } S;
extern u8 D_80149DC8[];
S *func_800C561C(S *arg0) { arg0->unkC = D_80149DC8; arg0->unk8 = 0; return arg0; }
