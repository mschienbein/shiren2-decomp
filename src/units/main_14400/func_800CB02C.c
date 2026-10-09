#include "common.h"
extern void func_800C55B0(s32 *value);
s32 func_800CB02C(s32 value)
{
    func_800C55B0(&value);
    return value;
}
