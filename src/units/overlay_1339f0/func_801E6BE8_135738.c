#include "func_801E6BE8_135738/types.h"

extern void func_801E6BC4_135714(s32 x, s32 y);

/* Centre a row of `columns` 16-pixel cells on the 640-wide surface. */
void func_801E6BE8_135738(s32 columns, s32 y) {
    func_801E6BC4_135714((640 - columns * 16) / 2, y);
}
