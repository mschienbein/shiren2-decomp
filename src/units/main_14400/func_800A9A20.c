#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

typedef unsigned char u8;
typedef unsigned short u16;

/* 0x18-byte resource record of the 21-record table at 0x80142B1C: five pointer-valued
 * words, then four metadata bytes at +0x14 (0x80142B30 is record 0's metadata). */
typedef struct {
    void *words[5];
    u8 metadata[4];
} Record800A9A20;


extern Record800A9A20 D_80142B1C[];

u8 func_800A9A20(void) {
    return D_80142B1C[D_80142F24.index].metadata[0];
}
