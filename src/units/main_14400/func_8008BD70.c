#include "common.h"

typedef unsigned char u8;

extern u8 D_801C3394;
extern void func_8005C870(u8 value);
extern void func_8005C814(u32 value0, u32 value1, u32 value2, u32 value3);
extern void func_8005C890(s32 arg0, s32 value, s32 count);
extern void func_8006E6D8(void);

void func_8008BD70(s32 count, s32 value) {
    s32 i;

    D_801C3394 = value;
    func_8005C870(1);
    func_8005C814(D_801C3394, D_801C3394, D_801C3394, 0);
    func_8005C890(1, value, count);
    for (i = 0; i < count; i++) {
        func_8006E6D8();
    }
}
