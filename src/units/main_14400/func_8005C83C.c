#include "common.h"

/* External word views shared with the frozen accepted setter. Original
 * signedness, containing allocations and historical API names are unknown. */
extern u32 D_80165940;
extern u32 D_80165944;
extern u32 D_80165948;
extern u32 D_8016594C;

u32 func_8005C83C(u32 *arg0, u32 *arg1, u32 *arg2, u32 *arg3)
{
    u32 result;

    *arg0 = D_80165940;
    *arg1 = D_80165944;
    *arg2 = D_80165948;
    result = D_8016594C;
    *arg3 = result;
    return result;
}
