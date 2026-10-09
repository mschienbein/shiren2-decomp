#include "common.h"
typedef unsigned short u16;
extern u16 func_800E08B0(void *obj);
s32 func_800E32F0(void *object)
{
    return func_800E08B0(object) == 0;
}
