#include "common.h"
typedef unsigned char u8;
/* func_800B52D0 returns a full int (addiu v0,v1,-249 at 0x800B52F0, no narrowing);
 * this wrapper narrows it to u8 (andi v0,v0,0xFF at 0x80042460). */
s32 func_800B52D0(s32 *pair);
u8 func_80042444(s32 a, s32 b) {
    s32 pair[2];
    pair[1] = a;
    pair[0] = b;
    return (u8)func_800B52D0(pair);
}
