#include "common.h"
typedef short s16;
typedef struct { s32 unk0; s16 unk4; char unk6[8]; s16 unkE; s32 unk10, unk14; char unk18[0xC]; s32 unk24, unk28, unk2C; char unk30[0x2C]; s32 unk5C; char unk60[8]; s32 unk68; } State;
extern s32 func_800748F8(s32, s32, s32, s32, s32, s32);
extern char D_801C3395[];
void func_80087ABC(State *arg0) {
    if (func_800748F8(arg0->unk14, arg0->unk24, arg0->unk5C, arg0->unk68, arg0->unk2C, arg0->unk28) != -1) D_801C3395[arg0->unk14] = 1;
    arg0->unkE = 1;
    arg0->unk4 = 4;
}
