#include "common.h"

u32 func_80072440(u32);
void func_8006AAF0(void *, u32, s32);
s32 func_8007248C(u32 arg0, void *arg1, s32 arg2) {
    u32 handle = func_80072440(arg0);
    s32 ret;
    if (handle != 0) {
        func_8006AAF0(arg1, handle, arg2);
        ret = 0;
    } else {
        ret = -1;
    }
    return ret;
}
