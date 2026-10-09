#include "common.h"

/* 0x18-byte resource record of the 21-record table at 0x80142B1C: five pointer-valued
 * words, then four metadata bytes at +0x14 (0x80142B30 is record 0's metadata). */
typedef struct { void *words[5]; unsigned char metadata[4]; } Record;
/* Whole 11-byte saved selection at 0x80142F24..0x80142F2E. */
typedef struct {
    unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08;
    signed char result;
    unsigned char field_0A;
} SelectionSave;
extern SelectionSave D_80142F24;
extern Record D_80142B1C[21];
/* first/second unused: the sole caller func_800CC288 passes its saved copy of the
 * D_80142F24/D_80142F25 cursor pair as bytes (lbu a0 at 0x800CC34C, lbu a1 at 0x800CC354), the
 * byte pair func_800A9C8C takes; this body reads D_80142F24 directly and ignores both. */
s32 func_800A9A44(unsigned char first, unsigned char second) {
    return D_80142B1C[D_80142F24.index].metadata[0] - D_80142B1C[D_80142F24.index].metadata[1];
}
