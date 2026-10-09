#include "common.h"
extern s32 D_801CA950, D_801CA954, D_801CA958;
extern s32 D_801CA95C;
s32 func_8012D940(s32 a, s32 b, u32 divisor, s32 percentage) {
    s32 period = (((a * b + divisor - 1) / divisor) / 184 + 1) * 184;
    s32 scaled;
    D_801CA950 = period;
    D_801CA954 = period - 184;
    D_801CA958 = period + 184;
    scaled = (u32)(period * percentage) / 100;
    D_801CA95C = scaled;
    scaled += 184;
    return period + scaled;
}
