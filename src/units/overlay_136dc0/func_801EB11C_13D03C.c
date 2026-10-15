#include "func_801EB11C_13D03C/types.h"
extern s32 func_800D5374(s32 index);
extern s32 func_800D5404(s32 index);

s32 func_801EB11C_13D03C(s32 ordinal)
{
    s32 count = 0;
    s32 i = 0;
    for (; i < 3; i++) {
        if (func_800D5374(i) && !func_800D5404(i)) {
            if (count == ordinal) {
                return i;
            }
            count++;
        }
    }
    if (ordinal == 4) {
        return count;
    }
    return -1;
}
