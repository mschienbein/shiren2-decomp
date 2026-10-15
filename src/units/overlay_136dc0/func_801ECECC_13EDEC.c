#include "func_801ECECC_13EDEC/types.h"

void *func_800A8CB0(s32 cell);
s32 func_800A8974(void *p);
void func_800A59A4(void *p);

static inline s32 flag3_clear(Cell801ECECC *cell) {
    return ((cell->flags_1E >> 3) & 1) ^ 1;
}

/* `cell != 0 ? &cell->link_84 : 0` is the project idiom for the null-guarded sub-object
 * address (canonical func_800DF61C, func_800B7574); measured: plain cell->link_84.active
 * gives 132 B / 13 differing words. */
void func_801ECECC_13EDEC(void) {
    s32 i = 30;

    for (;;) {
        Cell801ECECC *cell;

        if (--i == 0) {
            break;
        }
        cell = func_800A8CB0((u8)i);
        if (func_800A8974(cell)) {
            continue;
        }
        if (flag3_clear(cell)) {
            continue;
        }
        if ((cell != 0 ? &cell->link_84 : 0)->active == 0) {
            continue;
        }
        func_800A59A4(cell);
    }
}
