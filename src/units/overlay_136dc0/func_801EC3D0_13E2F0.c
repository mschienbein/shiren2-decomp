#include "func_801EC3D0_13E2F0/types.h"
extern u8 func_801E9F70_13BE90(s32 flag);
extern void func_801E9F90_13BEB0(s32 flag, u8 value);
extern u8 func_800C57A0(void *object);
extern u8 D_80147620[];

void func_801EC3D0_13E2F0(void)
{
    s32 flag = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
    s32 value = 0;
    u8 mode;
    flag <<= 8;
    flag += func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
    mode = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
    switch ((u32)mode) {
    case 0:
        value = 1;
        break;
    case 1:
        /* ODD_C: operation 1 explicitly clears the flag; the default is also
         * clear. ROM 0x13E374 tests this real operation before 0, 2 and 3.
         * Omitting this case produced 260 bytes / 61 differing words. */
        break;
    case 2:
        value = 1 - func_801E9F70_13BE90(flag);
        break;
    case 3:
        value = func_800C57A0(D_80147620) & 1;
        break;
    }
    func_801E9F90_13BEB0(flag, value);
}
