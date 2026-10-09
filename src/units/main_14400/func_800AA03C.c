#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;


/* 0x18-byte resource record of the 21-record table at 0x80142B1C: five pointer-valued
 * words, then four metadata bytes at +0x14 (0x80142B30 is record 0's metadata). */
typedef struct { void *words[5]; u8 metadata[4]; } Record800AA03C;


extern Record800AA03C D_80142B1C[];
s32 func_800AA03C(void) {
    u8 flags = D_80142F18.flags;
    if (flags & 8) {
        return 0;
    }
    switch (flags & 3) {
    case 1:
        return D_80142F24.count < D_80142B1C[D_80142F24.index].metadata[0] - 1;
    case 2:
        return D_80142F24.count != 0;
    }
    return 0;
}
