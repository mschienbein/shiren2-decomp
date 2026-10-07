#include "common.h"

extern s32 D_80165394;
extern s32 D_80165398;

s32 func_8005B058(void)
{
    s32 result = 4;

    if (D_80165394 == 1) {
        result = D_80165398;
    }
    return result;
}
