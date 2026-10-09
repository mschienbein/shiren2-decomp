#include "common.h"
s32 func_800A4198(s32 flags, s32 type) {
    if (type == 2) {
        return (flags & 0xC100) == 0;
    }
    return 0;
}
