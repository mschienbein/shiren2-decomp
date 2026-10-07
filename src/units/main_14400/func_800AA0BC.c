#include "common.h"

typedef unsigned char u8;

typedef signed char s8;

extern u8 D_80142F1B;
/* func_800A9C8C stores the cursor pair at offsets 0 and 1 from D_80142F24.
 * D_80142F25 names the second byte of this two-byte storage view. */
extern u8 D_80142F24[2];
extern s32 func_800AA03C(void);
extern void func_800A9C8C(u8 arg0, u8 arg1);

s8 func_800AA0BC(void) {
    s8 step;
    u8 *cursor;

    s32 notReady = func_800AA03C() != 1;

    if (notReady) {
        return 0;
    }
    switch (D_80142F1B & 3) {
        case 1:
            step = 1;
            break;
        case 2:
            step = -1;
            break;
        default:
            return 0;
    }
    cursor = &D_80142F24[1];
    *cursor += step;
    func_800A9C8C(cursor[-1], *cursor);
    return step;
}
