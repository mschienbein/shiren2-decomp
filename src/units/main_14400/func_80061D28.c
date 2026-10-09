#include "common.h"
extern s32 func_800419C4(void), func_800419E0(void), func_80041A0C(void), func_80042990(void);
extern void func_800740B4(s32), func_800740C0(s32), func_800740CC(s32), func_800740D8(s32);
extern void func_80061DD0(s32, s32, s32, s32), func_80061FB8(s32, s32, s32, s32);
void func_80061D28(s32 first, s32 second, s32 third, s32 fourth) {
    func_800740B4(func_800419C4());
    func_800740C0(func_800419E0());
    func_800740CC(func_80041A0C());
    func_800740D8(func_80042990());
    func_80061DD0(first, second, third, fourth);
    func_80061FB8(first, second, third, fourth);
}
