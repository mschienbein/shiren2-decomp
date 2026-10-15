#include "func_801E8B9C_13AABC/types.h"
extern void func_80045A24(s32 sound);
/* ODD_C: `pos` is an ordinary local base pointer (assigned once, used multiple
 * times), not a dead store; it keeps the local's address live across the switch. Direct aggregate
 * access, with either common or per-case commits, gives 192 bytes / 41
 * differing words. Pointer-returning getters, explicit pointer with a common
 * commit, and an inline dispatch helper give 196 bytes / 6 differing words.
 * The explicit pointer with per-case commits gives the original 196 bytes. */
void func_801E8B9C_13AABC(S06Widget *widget, s32 direction) {
    S06Point storage;
    S06Point *pos = &storage;
    *pos = widget->point34;
    switch (direction) {
    case 0:
        if (pos->x < widget->field20 - 1) pos->x++;
        else if (widget->field60 == 1) pos->x = 0;
        else return;
        widget->table4C->call84((u8 *)widget + widget->table4C->delta80, pos);
        func_80045A24(2);
        break;
    case 1:
        if (widget->point34.x > 0) pos->x--;
        else if (widget->field60 == 1) pos->x = widget->field20 - 1;
        else return;
        widget->table4C->call84((u8 *)widget + widget->table4C->delta80, pos);
        func_80045A24(2);
        break;
    default:
        return;
    }
}
