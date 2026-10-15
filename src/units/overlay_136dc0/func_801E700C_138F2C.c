#include "func_801E700C_138F2C/types.h"

void func_801E700C_138F2C(void)
{
    s32 i;
    for (i = 7; i != -1; i--) {
        OvlEffect *p = &D_801F6EAA_148DCA[i];
        if (p->b02) func_8008C194(p->b02 - 1);
    }
    func_801E6E4C_138D6C();
}
