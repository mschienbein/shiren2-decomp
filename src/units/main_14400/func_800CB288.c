#include "common.h"
typedef unsigned char u8;
typedef struct { char unk0[9]; u8 unk9; char unkA[0x1A]; s32 unk24; } State;
extern const u8 D_8015488C[8], D_80154894[8];
extern void func_800925E0(s32);
void func_800CB288(State *arg0, s32 arg1) {
    if (arg1) arg0->unk9 |= D_8015488C[0];
    else arg0->unk9 &= D_80154894[0];
    arg0->unk24 = arg1;
    func_800925E0(arg1);
}
