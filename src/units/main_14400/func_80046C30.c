#include "common.h"
s32 D_80138C50[4] = { 1, 2, 3, 10 };
s32 func_80072D54(void);
void func_80060C54(u32 mode);
void func_80046C30(s32 id) {
    if (func_80072D54() == 0) func_80060C54(D_80138C50[id]);
}
