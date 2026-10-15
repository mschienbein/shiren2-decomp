#include "func_801E6258_134DA8/types.h"

extern const u8 D_801E8130_136C80[];
extern const u8 D_80151E38[144];
extern const u8 D_80152968[];
extern void func_800D8FA8(void *object);

/* ODD_C: explicit derived-to-base destructor table transitions, also preserving
 * the original store schedule. Keeping only each final table produced 60 bytes
 * and 14 differing words, versus the ROM's 84 bytes.
 * ROM stores 801E6270/801E6288/801E6294 (owner), 801E6280/6284/628C (members). */
void func_801E6258_134DA8(OvlOwner *owner, s32 flags) {
    owner->menu.table_4C = D_801E8130_136C80;
    owner->third_288.menu.table_4C = D_80151E38;
    owner->second_17C.menu.table_4C = D_80151E38;
    owner->menu.table_4C = D_80152968;
    owner->first_05C.menu.table_4C = D_80151E38;
    owner->menu.table_4C = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(owner);
    }
}
