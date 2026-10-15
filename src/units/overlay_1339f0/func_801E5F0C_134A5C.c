#include "func_801E5F0C_134A5C/types.h"

void func_801E5A30_134580(List801E5A30 *list, Item801E5A30 *items, s32 count, s32 *origin, s32 style);

void func_801E5F0C_134A5C(Owner801E5E68 *self, Item801E5A30 *items384, s32 count384, s32 *origin384,
                   Item801E5A30 *items64, s32 count64, s32 *origin64) {
    s32 i;

    func_801E5A30_134580(&self->list384, items384, count384, origin384, D_80138EDC);
    func_801E5A30_134580(&self->list64, items64, count64, origin64, D_80138E88);
    for (i = 0; i < 2; i++) {
        self->list384.flags[i] = 0;
        self->list64.flags[i] = 0;
    }
    for (i = 0; i < count64; i++) {
        self->list384.flags[items64[i].value] = 1;
        self->list64.flags[items64[i].value] = 1;
    }
}
