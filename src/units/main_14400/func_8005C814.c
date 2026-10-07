#include "common.h"

/* Four observed 32-bit slots in the loader-cleared main-image BSS.
 * u32 preserves all input bits; original signedness and field names are unknown.
 * Some consumers alias the low bytes of the first three slots. No input range
 * is imposed here, and no shared storage is defined by this translation unit. */
extern u32 D_80165940;
extern u32 D_80165944;
extern u32 D_80165948;
extern u32 D_8016594C;

void func_8005C814(u32 value0, u32 value1, u32 value2, u32 value3)
{
    D_80165940 = value0;
    D_80165944 = value1;
    D_80165948 = value2;
    D_8016594C = value3;
}
