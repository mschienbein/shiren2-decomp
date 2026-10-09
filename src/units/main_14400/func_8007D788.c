#include "common.h"

typedef struct {
    u32 command;
    u32 parameter;
} DisplayCommand;

#define G_ENDDL 0xDF

extern s32 D_8013DFE4;
extern s32 D_8013DFE8;
extern s32 D_8013DFEC;
extern s32 D_8013DFF0;
extern s32 D_8013DFF4;
extern s32 D_8013DFF8;
extern s32 D_801A807C;
extern s32 D_801A8080;
extern s32 D_801A8084;
extern s32 D_801A8088;
/* Two 128-command display-list buffers. */
extern DisplayCommand D_801A80A0[2][128];

void func_8007D8B0(s32 value);
void func_8007D8BC(s32 value);

void func_8007D788(void)
{
    D_8013DFEC = -1;
    D_8013DFF0 = -1;
    D_8013DFF4 = -1;
    D_8013DFF8 = -1;
    D_8013DFE4 = 0x2C;
    D_801A807C = 0;
    D_801A8080 = 0;
    D_801A8084 = 0;
    D_801A8088 = 0;
    D_8013DFE8 = 0x23;
    func_8007D8B0(1);
    func_8007D8BC(0);
    D_801A80A0[0][0].command = G_ENDDL << 24;
    D_801A80A0[0][0].parameter = 0;
    D_801A80A0[1][0].command = G_ENDDL << 24;
    D_801A80A0[1][0].parameter = 0;
}
