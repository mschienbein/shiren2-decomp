#include "common.h"
typedef unsigned char u8;
typedef struct { s32 unk0, unk4; } Pair;
typedef struct { Pair pair; u8 unk8; char unk9[3]; s32 unkC, unk10, unk14; u8 unk18; } State;
void func_800C2CDC(State *arg0, Pair *arg1, u8 *arg2, s32 arg3, s32 arg4) {
    arg0->pair = *arg1;
    arg0->unk8 = *arg2;
    arg0->unkC = 0;
    arg0->unk10 = arg3;
    arg0->unk14 = arg4;
    arg0->unk18 = 0;
}
