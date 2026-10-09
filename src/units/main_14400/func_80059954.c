#include "common.h"
typedef signed char s8;
/* One 16-byte bounds object {min x, max x, min z, max z}: func_800598B4 stores all
 * four floats from the single D_80165438 base (+0x0/+0x4/+0x8/+0xC). */
extern float D_80165438[4];
void func_80059954(s8 *left, s8 *top, s8 *right, s8 *bottom) {
    *left = ((u32)D_80165438[0] >> 5) - 10;
    *top = ((u32)D_80165438[2] >> 5) - 10;
    *right = ((u32)D_80165438[1] >> 5) - 10;
    *bottom = ((u32)D_80165438[3] >> 5) - 10;
}
