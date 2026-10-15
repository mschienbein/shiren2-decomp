#include "common.h"

extern char D_8014AA90[];
void func_800547E0(s32 mode, char *fmt, void *args);

/* Both mapped callers pass integer 0x4F9 here, but this entry ignores it:
 * ROM 0x1B384..0x1B394 selects the fixed "%s" and starts at argument 3.
 * The integer preserves the unused argument slot, not a format pointer. */
void func_80048334(s32 mode, s32 unused_message, ...) {
    func_800547E0(mode, D_8014AA90, __builtin_next_arg(unused_message));
}
