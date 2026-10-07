#include "common.h"
typedef float f32;
extern s32 D_801CA6F4;
s32 func_8012BF6C(s32 range) {
    s32 i;
    for (i = 0; i < 8; i++) {
        s32 bits = D_801CA6F4 & 0x48000000;
        D_801CA6F4 <<= 1;
        if (bits == 0x48000000 || bits == 0x08000000) D_801CA6F4 |= 1;
    }
    return range * ((f32)D_801CA6F4 * (1.0f / 65536.0f) * (1.0f / 65536.0f));
}
