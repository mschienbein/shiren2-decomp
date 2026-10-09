#include "common.h"
extern s32 D_80138C40[4];
static inline s32 enabled(s32 (*state)[4]) { return (*state)[0]; }
static inline void set_enabled(s32 (*state)[4], s32 value) { (*state)[0] = value; }

extern void func_80048240(unsigned short, ...);
/* Virtual action slot +0x14 supplies the receiver, unused by this toggle. */
s32 func_800DEF10(void *unused_receiver) {
    s32 value = enabled(&D_80138C40) ^ 1;
    set_enabled(&D_80138C40, value);
    if (value) func_80048240(0x23D);
    else func_80048240(0x23E);
    return 1;
}
