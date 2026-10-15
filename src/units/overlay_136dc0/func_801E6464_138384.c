#include "func_801E6464_138384/types.h"

/* Overlay B ROM 0x147049: 00. Local ownership permits the sb delay slot. */
u8 D_801F5129_147049 = 0;

void func_801E6464_138384(s32 value)
{
    D_801F5129_147049 = value;
    func_801E6418_138338();
}
