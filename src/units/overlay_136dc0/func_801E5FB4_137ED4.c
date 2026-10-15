#include "func_801E5FB4_137ED4/types.h"

void func_800265E0(void *dst, s32 length);

void func_801E5FB4_137ED4(void) {
    s32 i;

    func_800265E0(D_801F6714_148634, sizeof(D_801F6714_148634));
    for (i = 9; i != -1; i--) {
        D_801F6714_148634[i].x6 = 0xFFFF;
    }
}
