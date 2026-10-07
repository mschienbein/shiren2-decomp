#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

extern u32 D_80169A44;
extern s32 D_8013B758;
extern s32 D_8013B75C;
extern void (*D_8013B7C0[])(void);
extern u8 D_801E4E48[];
extern u8 D_001339F0[];
extern u8 D_136DC0[];
extern u8 D_00148460[];
s32 func_801E4EA0(void);
void func_80025DCC(void *romStart, void *vram, s32 size);
void func_80054DB0(void);
void func_800553DC(void);
void func_80056FB8(void);
void func_8005CBC4(void);
void func_8006126C(float x, float y);
void func_80061650(void);
void func_8006A9AC(void);
u8 func_8006C508(u8 value);
void func_8006CD14(void);
void func_8006E6D8(void);
void func_8006E8A0(void);
void func_8006EA18(void *arg);
void func_80071B60(void);
void func_80072664(void);
void func_800744DC(void);
void func_8007D58C(void);
void func_8008BEB8(void);
void func_80060C54(u32 mode) {
    if (D_80169A44 == mode) {
        return;
    }
    if (D_8013B75C != 0 && D_8013B758 == 0) {
        func_8006E6D8();
    }
    func_8006CD14();
    func_800744DC();
    func_80061650();
    func_8006E8A0();
    func_80071B60();
    func_800553DC();
    func_8005CBC4();
    func_80054DB0();
    func_80056FB8();
    func_80072664();
    func_8008BEB8();
    func_8007D58C();
    func_8006EA18(D_801E4E48);
    func_8006A9AC();
    func_8006126C(0.0f, 0.0f);
    if (D_80169A44 == 2) {
        func_80025DCC(D_136DC0, (void *)func_801E4EA0, D_00148460 - D_136DC0);
    } else if (mode == 2) {
        func_80025DCC(D_001339F0, (void *)func_801E4EA0, D_136DC0 - D_001339F0);
    }
    D_80169A44 = mode;
    if (mode < 11 && D_8013B7C0[mode] != 0) {
        D_8013B7C0[mode]();
        return;
    }
    func_8006C508(1);
}
