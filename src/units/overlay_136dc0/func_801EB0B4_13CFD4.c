#include "func_801EB0B4_13CFD4/types.h"
extern s32 func_800D5404(s32 id);

s32 func_801EB0B4_13CFD4(s32 ordinal)
{
    s32 i = 0;
    s32 count = 0;
    for (; i < 3; i++) {
        if (func_800D5404(i)) {
            if (count == ordinal) {
                return i;
            }
            count++;
        }
    }
    return -1;
}
