#include "func_801EC374_13E294/types.h"
extern u8 func_801E9F70_13BE90(s32 flag);
extern void func_801E9F90_13BEB0(s32 flag, u8 value);
extern s32 func_800D7FE8(void);
extern void *func_800C9E10(void);
extern void func_800CAC04(void *object);

void func_801EC374_13E294(void)
{
    void *object;
    if (!func_801E9F70_13BE90(0x49) && func_800D7FE8()) {
        func_801E9F90_13BEB0(0x49, 1);
        object = func_800C9E10();
        if (object) {
            func_800CAC04(object);
        }
    }
}
