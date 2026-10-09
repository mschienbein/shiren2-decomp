#include "common.h"
typedef unsigned char u8;
extern s32 D_8016DB18, D_8016DB54;
extern u32 D_8016DB2C, D_8016DB30, D_8016DB34;
extern void *D_8016DB38[2], *D_8016DB40[2], *D_8016DB48[2];
extern void *func_8006A8D8(char *name, u32 size);
extern void *D_801D2550;
extern void *D_8013B80C;
extern void *D_801DFF7C;
extern void *D_801DE97C;
extern void *D_801D85A8;
extern void *D_801D2558;
extern void *D_8013C760;
extern void *D_8013C764;
extern void *D_801D9350;
extern void *D_801D40CC;
extern void *D_801D2564;
extern void *D_801D934C;
extern void *D_801D2C08;
extern void *D_801D40D4;
extern void *D_801E02A4;
extern void *D_801D2560;
extern void *D_801D2C00;
extern void *D_801D2554;
extern const char D_8014C460[];
extern const char D_8014C474[];
extern const char D_8014C488[];
extern const char D_8014C498[];
extern const char D_8014C4A8[];
extern const char D_8014C4B8[];
extern const char D_8014C4C8[];
extern const char D_8014C4DC[];
void func_800612D4(s32 mode) {
    D_8016DB18 = 0;
    switch (mode) {
    default: case 0: {
        u8 *base;
        D_8016DB54 = 0;
        base = func_8006A8D8((char *)D_8014C460, 0x41D18);
        D_801D2550 = base + 0x79D0;
        D_8013B80C = base;
        D_801DFF7C = base;
        D_801DE97C = base;
        D_801D85A8 = base;
        D_801D2558 = base;
        D_8013C760 = base;
        D_801D9350 = base + 0x379D0;
        D_801D40CC = base + 0x3D150;
        D_801D2564 = base + 0x40800;
        D_801D934C = base + 0x417A0;
        D_801D2C08 = base + 0x880;
        D_801D40D4 = base + 0x20880;
        D_801E02A4 = base + 0x3A710;
        D_801D2560 = base + 0x39798;
        D_801D2C00 = base + 0x3D728;
        D_801D2554 = base + 0x3E6C8;
        D_8013C764 = base + 0x25800;
        D_8016DB2C = 0xC00;
        D_8016DB30 = 0xC00;
        D_8016DB34 = 0xC00;
        break;
    }
    case 1: {
        u8 *base;
        D_8016DB54 = 1;
        base = func_8006A8D8((char *)D_8014C474, 0x80D90);
        D_801D2550 = base + 0xC700;
        D_8013B80C = base;
        D_801DFF7C = base;
        D_801DE97C = base;
        D_801D85A8 = base;
        D_801D2558 = base;
        D_801D9350 = base + 0x6C700;
        D_801D40CC = base + 0x77600;
        D_801D2564 = base + 0x7E360;
        D_801D934C = base + 0x802A0;
        D_801D2C08 = base + 0x880;
        D_801D40D4 = base + 0x20880;
        D_801E02A4 = base + 0x3A710;
        D_801D2560 = base + 0x39798;
        D_801D2C00 = base + 0x3D728;
        D_801D2554 = base + 0x3E6C8;
        D_8016DB2C = 0x1800;
        D_8016DB30 = 0xC00;
        D_8016DB34 = 0;
        break;
    }
    }
    D_8016DB38[0] = func_8006A8D8((char *)D_8014C488, D_8016DB2C * 8);
    D_8016DB38[1] = func_8006A8D8((char *)D_8014C498, D_8016DB2C * 8);
    D_8016DB40[0] = func_8006A8D8((char *)D_8014C4A8, D_8016DB30 * 8);
    D_8016DB40[1] = func_8006A8D8((char *)D_8014C4B8, D_8016DB30 * 8);
    if (D_8016DB34) {
        D_8016DB48[0] = func_8006A8D8((char *)D_8014C4C8, D_8016DB34 * 8);
        D_8016DB48[1] = func_8006A8D8((char *)D_8014C4DC, D_8016DB34 * 8);
    }
}
