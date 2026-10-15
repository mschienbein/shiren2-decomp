#include "func_801E69E8_138908/types.h"

void func_801E69E8_138908(u8 mode)
{
    s32 i;
    for (i = 28; i != -1; i--) {
        HostUnit *p = func_8007946C(0, i);
        if (p->h02 != -1) {
            if (mode) {
                if (p->b07 != 2) func_80075E5C(i, 0);
            } else {
                if (!p->b09) func_80075E5C(i, 5);
            }
        }
    }
}
