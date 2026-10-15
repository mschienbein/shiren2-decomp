#include "func_801F2888_1447A8/types.h"

static inline u8 table_get(s32 index) {
    return D_801F5228_147148[index];
}

/* Same byte-table read, widened to a word. Case 0x18 keeps its level in a
 * u32 local loaded through this accessor, separate from the three u8
 * table_get() reads in its range condition. */
static inline u32 table_word(s32 index) {
    return D_801F5228_147148[index];
}

/* The extracted bit is exactly 0/1. Complement it locally, without changing
 * any exported predicate's integer contract. */
static inline s32 selection_flag2_clear(void) {
    return ((D_80142F18.flags >> 2) & 1) ^ 1;
}

s32 func_801F2888_1447A8(u8 kind) {
    u32 level;

    if (func_801F2E30_144D50(kind)) {
        return 0;
    }
    if (func_800B74C0(kind)) {
        return 0;
    }
    if (table_get(2) >= 0xFD && selection_flag2_clear()) {
        return 0;
    }
    switch (kind) {
    case 0x1A:
        if (func_801E9F70_13BE90(0x25)) {
            return 0x114;
        }
        break;
    case 0x1B:
        if (func_801E9F70_13BE90(0x23)) {
            return 0x10F;
        }
        break;
    case 0x1C:
        if (func_801E9F70_13BE90(0x24)) {
            return 0x111;
        }
        break;
    case 0x18:
        level = table_word(2);
        /* ROM 0x801F2974..0x801F2998 tests table[2] against 100, 0xA6, 100
         * again, then 0xFD. The retest of 100 is dead code in the ROM itself,
         * so the original condition held a redundant ">= 100" disjunct
         * [inference from the emitted compares]. The whole condition is
         * equivalent to level >= 100. */
        if ((table_get(2) >= 100 && table_get(2) < 0xA6) || table_get(2) >= 100) {
            if (level < 0xFD && func_801E9F70_13BE90(0x29)) {
                break;
            }
            return 0x116;
        }
        break;
    case 0x19:
        level = table_get(2);
        if (level != 0xDC && level >= 0xA0) {
            return 0x11F;
        }
        break;
    }
    return 0;
}
