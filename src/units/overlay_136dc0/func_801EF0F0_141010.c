#include "func_801EF0F0_141010/types.h"
extern void func_801E57E4_137704(s32 frames);
extern s32 func_801E5858_137778(void);
extern s32 func_801E54B8_1373D8(s32 frames, s32 mode);
extern s32 D_80138BF0;
extern void func_8004633C(void);

void func_801EF0F0_141010(void)
{
    func_801E57E4_137704(8);
    while (func_801E5858_137778()) {
        func_801E54B8_1373D8(1, 0);
    }
    D_80138BF0 = 1;
    func_8004633C();
}
