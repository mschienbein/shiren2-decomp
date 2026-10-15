#include "func_801E817C_13A09C/types.h"

extern u8 D_801F5128_147048;
extern u8 D_801F512A_14704A;
extern void func_8005AD38(s32 mode);
extern void func_8005B0A8(u32 channel, s32 duration, float value);

void func_801E817C_13A09C(u32 channel, s32 duration, f32 value) {
    if (D_801F5128_147048 == 0) {
        func_8005AD38(D_801F512A_14704A != 0);
        D_801F5128_147048++;
    }
    func_8005B0A8(channel, duration, value);
}
