#include "common.h"

extern s32 func_800A44F4(void *self, void *target);

s32 func_800EE958(void *self, void *target)
{
    return func_800A44F4(self, target) != 1;
}
