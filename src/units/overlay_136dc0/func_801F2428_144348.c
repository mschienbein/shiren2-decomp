#include "func_801F2428_144348/types.h"

/* ODD_C: test a byte's selected mask, not an identity wrapper. The original
 * author's direct D_8015488C[7]/[6]/[0] alternative shared a base register
 * (23 differing words); this inline mask operation kept separate ROM-style
 * address loads. That measurement belongs to the preserved author baseline. */
static inline s32 bit_test(u8 flags, s32 bit) {
    return flags & D_8015488C[bit];
}

s32 func_801F2428_144348(void) {
    Record80147680 *record;
    s32 id;

    func_800A9C5C(0);
    func_801EA004_13BF24();
    record = func_800C9E10();
    if (record == 0) {
        id = 0x4A0;
    } else if (!bit_test(record->flags7, 7)) {
        id = 0x16;
    } else if (!bit_test(record->flags7, 6)) {
        func_801F2254_144174(0x2D0, 0);
        return 1;
    } else if (bit_test(record->flags8, 0)) {
        id = 0x4A0;
    } else {
        id = 0x17;
    }
    func_801F2254_144174(id, 0);
    return 0;
}
