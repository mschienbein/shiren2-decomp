#include "common.h"
extern s32 D_8016DB18;
extern void func_8006178C(s32 value);
extern void func_80061724(void);
extern void func_800649F0(void);
extern void func_80067620(void);
extern void func_80069A60(void);
void func_80061290(void) {
    D_8016DB18 = 0;
    func_8006178C(1);
    func_80061724();
    func_800649F0();
    func_80067620();
    func_80069A60();
}
