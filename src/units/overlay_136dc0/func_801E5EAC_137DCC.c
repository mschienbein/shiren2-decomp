#include "func_801E5EAC_137DCC/types.h"

typedef struct {
    s8 x;
    s8 y;
} BytePair;

u8 func_80042498(s32 kind);
void func_80051E9C(s16 id, BytePair pair);

void func_801E5EAC_137DCC(void) {
    s32 i;
    D_801F6714_view *slot;
    D_801F6714_view *periods;
    BytePair pair;

    /* ODD_C: both cursors visit the same ten records. Keeping the period
     * cursor separate reproduces the ROM's two descending address streams.
     * Measured alternatives: inherited single cursor = 56 differing words;
     * local-table indexing = 59; descending cursor plus indexed x0 = 30.
     * Neither cursor is dereferenced at its initial one-past position. */
    i = 10;
    slot = D_801F6714_148634 + i;
    periods = D_801F6714_148634 + i;
    while (--i != -1) {
        --slot;
        --periods;
        if (slot->x6 == 0xFFFF) {
            continue;
        }
        if (++slot->x1 <= periods->x0) {
            continue;
        }
        if (slot->x2 >= func_80042498(0xFF)) {
            pair.x = slot->x3;
            pair.y = slot->x4;
            func_80051E9C(slot->x6, pair);
        }
        slot->x1 = 0;
        if (slot->x5 != 0xFF) {
            if (--slot->x5 == 0) {
                slot->x6 = 0xFFFF;
            }
        }
    }
}
