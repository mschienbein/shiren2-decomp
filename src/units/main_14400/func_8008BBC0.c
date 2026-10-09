#include "common.h"
extern void func_8007D8A0(s32), func_8007D3A8(s32);
extern void func_8005C870(unsigned char);
extern void func_8005C814(u32, u32, u32, u32);
extern unsigned char func_80042060(void);
extern void func_800557E0(void), func_80055874(void), func_8006E6D8(void);
extern void func_800559F8(s32), func_80076714(s32);
extern void func_8005C890(s32, s32, s32);
void func_8008BBC0(s32 enabled) {
    s32 i;
    func_8007D8A0(1); func_8007D3A8(0); func_8005C870(2); func_8005C814(0, 0, 0, 255);
    if (enabled) { if (func_80042060()) return; func_800557E0(); func_800559F8(1); for (i = 0; i < 24; ++i) { func_80076714(0); func_8006E6D8(); } }
    func_8005C890(0, 0, 8);
    for (i = 0; i < 16; ++i) { func_80076714(0); func_8006E6D8(); }
    func_8005C814(0, 0, 0, 0); func_800559F8(3);
    for (i = 15; i >= 0; --i) { func_80076714(0); func_8006E6D8(); }
    func_80055874(); func_8007D8A0(0); func_80076714(0); func_8006E6D8(); func_8006E6D8(); func_8007D3A8(1); func_8005C870(0); func_8006E6D8();
}
