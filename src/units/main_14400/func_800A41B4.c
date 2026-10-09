#include "common.h"

s32 func_800A41B4(s32 arg0, s32 arg1) {
    if (arg1 == 2) {
        return (arg0 & 0xC000) == 0;
    }
    return 0;
}
