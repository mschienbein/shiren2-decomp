#include "common.h"

extern u32 D_8016FD34;

u32 func_80069A10(void)
{
    D_8016FD34 = D_8016FD34 * 214013 + 2531011;
    return D_8016FD34 >> 16;
}
