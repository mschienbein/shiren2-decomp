#include "common.h"
typedef unsigned char u8;
extern u8 D_801C3394;
extern void func_8005C870(unsigned char);
extern void func_8005C814(u32, u32, u32, u32);
extern void func_8005C890(s32, s32, s32);
extern void func_8006E6D8(void);
void func_8008BCF4(s32 count) {
    s32 i;
    func_8005C870(1);
    i = 0;
    func_8005C814(D_801C3394, D_801C3394, D_801C3394, 255);
    func_8005C890(0, D_801C3394, count);
    for (; i < count; i++) func_8006E6D8();
}
