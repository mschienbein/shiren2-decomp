#include "common.h"

typedef signed char s8;
extern unsigned char D_801546EC[];

s32 func_800D18A8(s8 *values) {
    s32 count = 0;
    s32 sum = 0;
    s32 i;
    for (i = 0; i < 5; i++) {
        s32 value = values[i];
        if (value == -2) {
            return 0;
        }
        if (value >= 0) {
            count++;
            sum += D_801546EC[value];
        }
    }
    return sum / count;
}
