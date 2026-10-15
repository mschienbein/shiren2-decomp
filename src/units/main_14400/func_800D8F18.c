#include "common.h"

extern void func_800D8F10(s32 status);

/* Never returns: hands status -1 to the exit spin in func_800D8F10. */
void func_800D8F18(void) {
    func_800D8F10(-1);
    for (;;) {
    }
}

/* Separate 8-byte entry after func_800D8F18 (its own catalogue entry after the boundary
 * revision). Its sole direct caller, func_801EF7F0 in overlay_136dc0 (jal at 0x801EF8C0), passes
 * the address of the void(void) collection destructor func_801EF7A0 in a0 and discards v0. This
 * body never reads the callback and always returns 0. */
s32 func_800D8F30(void (*callback)(void)) {
    return 0;
}
