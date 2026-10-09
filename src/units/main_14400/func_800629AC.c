#include "common.h"

typedef struct Gfx { u32 command, argument; } Gfx;
extern u32 D_8016DB18;
extern s32 D_8016DB28, D_8013B808;
extern void *D_8013B80C; /* working-storage allocation base (func_800612D4); only null-tested */
/* func_800615A0 allocates each pair, and func_80061650 clears two slots. */
extern Gfx *D_8016DB38[2], *D_8016DB40[2], *D_8016DB48[2];
extern Gfx *D_801DEAB0, *D_801D2C24, *D_801D2C04;
extern Gfx D_010000B8[];
extern void func_800659B8(void);
extern void func_80068BC8(void);
extern void func_80069BFC(void);
extern void func_800611F8(s32 index, Gfx *commands);

void func_800629AC(void)
{
    if (!D_8016DB18 || !D_8016DB28 || !D_8013B80C)
        return;
    D_801DEAB0 = D_8016DB38[D_8013B808];
    D_801D2C24 = D_8016DB40[D_8013B808];
    D_801D2C04 = D_8016DB48[D_8013B808];
    {
        Gfx *packet = D_801DEAB0++;
        packet->command = 0xDE000000;
        packet->argument = (u32)D_010000B8;
    }
    {
        Gfx *packet = D_801D2C24++;
        packet->command = 0xDE000000;
        packet->argument = (u32)D_010000B8;
    }
    if (D_801D2C04) {
        Gfx *packet = D_801D2C04++;
        packet->command = 0xDE000000;
        packet->argument = (u32)D_010000B8;
    }
    switch (D_8016DB18) {
    case 1: case 2: func_800659B8(); break;
    case 3: func_80068BC8(); break;
    case 4: func_80069BFC(); break;
    }
    {
        Gfx *packet = D_801DEAB0++;
        packet->command = 0xDF000000;
        packet->argument = 0;
    }
    {
        Gfx *packet = D_801D2C24++;
        packet->command = 0xDF000000;
        packet->argument = 0;
    }
    if (D_801D2C04) {
        Gfx *packet = D_801D2C04++;
        packet->command = 0xDF000000;
        packet->argument = 0;
    }
    func_800611F8(0, D_8016DB38[D_8013B808]);
    func_800611F8(5, D_8016DB40[D_8013B808]);
    if (D_801D2C04)
        func_800611F8(4, D_8016DB48[D_8013B808]);
    D_8013B808 ^= 1;
}
