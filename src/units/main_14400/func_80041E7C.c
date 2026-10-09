#include "common.h"

/* Whole menu-system object; its signed flags byte is at +5. */
extern signed char D_80140160[];
extern s32 func_80046240(void);

s32 func_80041E7C(void)
{
    if (func_80046240()) {
        return 1;
    }
    return D_80140160[5] == 1;
}
