#include "func_801E63BC_1382DC/types.h"

/* Big-endian low-byte loads at 801E63E8/63F0/63F8/6400. */
void func_801E63BC_1382DC(s32 index, u32 *channels)
{
    HostUnit *p = func_8007946C(0, index);
    if (p != 0) {
        p->b70 = channels[0];
        p->b71 = channels[1];
        p->b72 = channels[2];
        p->b73 = channels[3];
    }
}
