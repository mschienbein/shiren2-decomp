#include "common.h"
typedef unsigned char u8;
extern u8 D_801CA66A[6];
extern u8 func_800A8C00(void *actor);
/* owner: receiver supplied by func_80126480's call; unused here. */
void func_801263B8(void *owner, void *unit) {
    u32 id = func_800A8C00(unit);
    if (id != 0xFF) D_801CA66A[id >> 3] |= 1 << (id & 7);
}
