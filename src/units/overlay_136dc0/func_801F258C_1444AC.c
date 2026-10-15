#include "func_801F258C_1444AC/types.h"

/* ODD_C: read the selected event byte. The original author's direct
 * D_801F5228_147148[3]/[7] alternative shared a base register (62 differing words);
 * the inline indexed load kept the ROM's separate address loads. This is
 * a preserved baseline measurement, not a new claim about the repaired ABI. */
static inline u8 table_get(s32 index) {
    return D_801F5228_147148[index];
}

s32 func_801F258C_1444AC(s32 kind, s32 query) {
    s32 id = 0;

    switch (kind) {
    case 0x53:
        if (D_80142F18.mode == 0x4E && table_get(2) < 0xFD) {
            id = 0x6B;
        }
        break;
    case 0x54:
        if (D_80142F18.mode == 0x4D && table_get(3) < 0x1E) {
            if (table_get(7) < 3) {
                func_801E8CF4_13AC14(7, table_get(7) + 1);
            }
            if (table_get(7) >= 3) {
                id = 0x60;
            } else if (query) {
                return 1;
            }
        }
        break;
    case 0x55:
        if (D_80142F18.mode == 0x79) {
            if (table_get(2) >= 0xFD) {
                id = 0x21B;
            } else {
                id = 0x1AD;
            }
            func_800C96FC();
        }
        break;
    case 0x56:
        if (D_80142F18.mode == 0x72) {
            if (table_get(2) >= 0xFD) {
                id = 0x21E;
            } else {
                id = 0x1BE;
            }
            func_800C96FC();
        }
        break;
    }
    if (!query && id != 0) {
        func_801F2254_144174(id, 0);
    }
    return id != 0;
}
