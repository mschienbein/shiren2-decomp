#include "func_801EB020_13CF40/types.h"
extern OvlProfile *func_800C9E10(void);
extern void func_800CADBC(void *profile);
extern const u8 D_8015488C[8];
extern const u8 D_80154894[8];

void func_801EB020_13CF40(s32 flag, s32 enabled)
{
    OvlProfile *profile = func_800C9E10();
    if (profile) {
        u8 *flags = profile->flags07;
        if (enabled) {
            flags[flag >> 3] |= D_8015488C[flag & 7];
        } else {
            flags[flag >> 3] &= D_80154894[flag & 7];
        }
        func_800CADBC(profile);
    }
}
