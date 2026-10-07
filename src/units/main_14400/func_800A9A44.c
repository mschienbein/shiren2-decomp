#include "common.h"

/* 0x18-byte resource record of the 21-record table at 0x80142B1C: five pointer-valued
 * words, then four metadata bytes at +0x14 (0x80142B30 is record 0's metadata). */
typedef struct { void *words[5]; unsigned char metadata[4]; } Record;
extern unsigned char D_80142F24;
extern Record D_80142B1C[];
s32 func_800A9A44(void) {
    return D_80142B1C[D_80142F24].metadata[0] - D_80142B1C[D_80142F24].metadata[1];
}
