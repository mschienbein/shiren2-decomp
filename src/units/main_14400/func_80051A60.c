#include "common.h"
extern void func_80051CE8(void), func_800528BC(void), func_80053100(void);
extern void *func_8006A8D8(const char *, u32), *func_8006A934(const char *, u32), *func_8006AC90(void);
extern void func_8006AAF0(void *, u32, s32);
extern void func_80129C2C(void *), func_8012A75C(void *, void *), func_8012A910(void *), func_8012A9BC(void *, void *), func_8012A9CC(void *);
extern unsigned char D_008CA860[], D_008CA860_end[], D_008D4290[], D_00AEAE00[], D_00AEAE00_end[], D_00AF67E0[], D_00E4F660[], D_00E4F660_end[], D_00E53B20[], D_00E53DB0[];
extern unsigned char D_801397C0[];
extern const char D_8014BF9C[], D_8014BFA8[], D_8014BFB4[], D_8014BFC0[], D_8014BFCC[], D_8014BFDC[], D_8014BFE8[];
extern void *D_80161640, *D_801D2568, *D_801D40D0, *D_801D4D04, *D_801D9330, *D_801D9354, *D_801DEA04, *D_801DEA08;
extern unsigned char D_8016166C, D_8016166D, D_8016166E, D_8016166F, D_80161670;
typedef struct { s32 field_0, field_4; void *field_8; u32 field_C; void *field_10; u32 field_14; void *field_18, *field_1C, *field_20; s32 field_24, field_28, field_2C, field_30, field_34, field_38, field_3C; void *field_40; } Config;
/* local-arithmetic-qualification: these linker-defined cartridge boundaries
   denote device offsets, not separately allocated C array elements.
   Each resource has its own end symbol (D_<start>_end, proposed linker aliases):
   the end of one resource and the start of the next are distinct symbols at one
   address, so the original rematerializes them instead of reusing one constant.
   Config is the libmus musConfig passed to func_80129C2C. */
void func_80051A60(void) {
    Config config;
    D_80161640 = func_8006AC90();
    func_800528BC();
    D_801D9330 = func_8006A934(D_8014BF9C, 0x1F090);
    D_801D40D0 = func_8006A8D8(D_8014BFA8, 0x9A30);
    D_801D9354 = func_8006A8D8(D_8014BFB4, 0x6860);
    D_801D4D04 = func_8006A8D8(D_8014BFC0, 0x420);
    D_801D2568 = func_8006A8D8(D_8014BFCC, 0xB9E0);
    D_801DEA04 = func_8006A8D8(D_8014BFDC, 0x44C0);
    D_801DEA08 = func_8006A8D8(D_8014BFE8, 0x2A0);
    func_8006AAF0(D_801D40D0, (u32)D_008CA860, (u32)D_008CA860_end - (u32)D_008CA860);
    func_8006AAF0(D_801D2568, (u32)D_00AEAE00, (u32)D_00AEAE00_end - (u32)D_00AEAE00);
    func_8006AAF0(D_801DEA04, (u32)D_00E4F660, (u32)D_00E4F660_end - (u32)D_00E4F660);
    func_8006AAF0(D_801DEA08, (u32)D_00E53B20, (u32)D_00E53DB0 - (u32)D_00E53B20);
    config.field_4 = 0x18;
    config.field_C = 0x50;
    config.field_24 = 0x40;
    config.field_1C = &D_008D4290;
    config.field_2C = 0x7D00;
    config.field_28 = 0x100;
    config.field_0 = 0;
    config.field_8 = 0;
    config.field_10 = D_801D9330;
    config.field_14 = 0x1F090;
    config.field_18 = D_801D40D0;
    config.field_20 = D_801DEA04;
    config.field_30 = 0x1000;
    config.field_34 = 1;
    config.field_38 = 0x34;
    config.field_3C = 0x240;
    func_8012A9CC(D_801397C0);
    func_80129C2C(&config);
    func_8012A75C(D_801D2568, &D_00AF67E0);
    func_8012A910(D_801DEA08);
    func_8012A9BC(D_801DEA04, D_801D2568);
    func_8012A9BC(D_801DEA08, D_801D40D0);
    D_8016166D = 0;
    D_8016166C = 1;
    D_8016166E = 0;
    D_8016166F = 0;
    D_80161670 = 0;
    func_80053100();
    func_80051CE8();
}
