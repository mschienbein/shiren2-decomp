#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* 0x18-byte resource record of the 21-record table at 0x80142B1C: five pointer-valued
 * words, then four metadata bytes at +0x14 (0x80142B30 is record 0's metadata). */
typedef struct {
    void *words[5];
    u8 metadata[4];
} Record800A9A20;

extern u8 D_80142F24;
extern Record800A9A20 D_80142B1C[];

u8 func_800A9A20(void) {
    return D_80142B1C[D_80142F24].metadata[0];
}
