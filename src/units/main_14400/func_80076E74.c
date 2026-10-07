#include "common.h"

s32 func_800625FC(s32 arg0, s32 arg1);
s32 func_80062554(s32 arg0, s32 arg1);

s32 func_80076E74(s32 arg0, s32 arg1) {
    if (func_800625FC(arg0, arg1) & 0x80) {
        return -0x20;
    }
    return func_80062554(arg0, arg1);
}
