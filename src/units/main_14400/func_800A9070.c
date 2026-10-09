#include "common.h"

typedef unsigned char u8;

/* 0xE4-byte record table indexed by the iterator position. */
typedef struct {
    u8 pad00[0xA];
    u8 kind;
    u8 pad0B[0xE4 - 0xB];
} Record800A9070;

/* Unit iterator prefix: slot index at +0, Pair pointer at +4 (func_800A9204). */
typedef struct {
    s32 index;
    void *position_04;
} Iterator;

extern u8 D_801C51A4[];
extern const u8 D_8015488C[8];
extern Record800A9070 D_801C36EC[30]; /* Includes the fallback slot at D_801C50C0. */

/* Bit i of a byte-packed flag set (D_8015488C holds the per-bit masks). */
static inline s32 test_bit(u8 *bits, s32 i) {
    return bits[i >> 3] & D_8015488C[i & 7];
}

/* Kind byte of record i. */
static inline u8 record_kind(Record800A9070 *records, s32 i) {
    return records[i].kind;
}

/* Advance to the next enabled record of the given kind; the end slot matches kind 0x17. */
s32 func_800A9070(Iterator *it, s32 kind) {
    for (;;) {
        s32 i = it->index;

        if (i >= 29) {
            break;
        }
        if (test_bit(D_801C51A4, i) && record_kind(D_801C36EC, i) == kind) {
            return 1;
        }
        it->index++;
    }
    if (it->index == 29 && kind == 0x17) {
        return 1;
    }
    return 0;
}
