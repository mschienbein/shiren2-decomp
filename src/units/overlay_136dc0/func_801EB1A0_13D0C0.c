#include "func_801EB1A0_13D0C0/types.h"
/* ROM 0x13D0E4 passes the full signed loop index; agrees with s32 func_800D5484(s32)
 * (src/units/main_14400/func_800D5484.c:7). */
extern s32 func_800D5484(s32 index);

s32 func_801EB1A0_13D0C0(s32 ordinal)
{
    s32 i = 0;
    s32 count = 0;
    for (; i < 3; i++) {
        if (func_800D5484(i)) {
            if (count == ordinal) {
                return i;
            }
            count++;
        }
    }
    return -1;
}
