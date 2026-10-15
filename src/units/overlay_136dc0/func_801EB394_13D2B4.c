#include "func_801EB394_13D2B4/types.h"
/* ROM 0x13D2BC..0x13D2CC: lw followed by unsigned division by 10000. */
extern u32 D_80142934;
extern u8 func_801E8CF4_13AC14(u8 index, u8 value);

void func_801EB394_13D2B4(void)
{
    u32 value = D_80142934;
    u32 high = value / 10000;
    u32 middle;
    func_801E8CF4_13AC14(5, high);
    middle = value / 100;
    func_801E8CF4_13AC14(6, middle - high * 100);
    func_801E8CF4_13AC14(7, value - middle * 100);
}
