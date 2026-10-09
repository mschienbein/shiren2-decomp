#include "common.h"
typedef struct { u32 command, parameter; } DisplayCommand;
extern s32 D_801A8078, D_801A8098;
extern DisplayCommand D_801A80A0[2][128];
extern void func_8007D84C(s32), func_8007D8B0(s32), func_8007D8BC(s32), func_8007D8A0(s32);
void func_8007D720(void) {
    func_8007D84C(0);
    func_8007D8B0(1);
    func_8007D8BC(0);
    func_8007D8A0(0);
    D_801A8078 = 1;
    D_801A8098 = 2;
    D_801A80A0[0][0].command = 0xdf000000;
    D_801A80A0[0][0].parameter = 0;
    D_801A80A0[1][0].command = 0xdf000000;
    D_801A80A0[1][0].parameter = 0;
}
