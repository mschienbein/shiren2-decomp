#include "common.h"
/* Whole 0x26-byte floor record at D_80142EF0 (0x80142EF0..0x80142F15): func_800ABBA0
 * fills it with one func_8006AC30 copy (stride 0x26, count 1); bytes are read with lbu
 * at +0x00..+0x25 and the halfword at +0xE with lhu (func_800AB044). */
typedef struct {
    unsigned char field_00, field_01, field_02, field_03, field_04, field_05, field_06, field_07;
    unsigned char field_08, field_09, field_0A, field_0B, field_0C, field_0D;
    unsigned short field_0E;
    unsigned char field_10, field_11, field_12, field_13, field_14, field_15, field_16, field_17;
    unsigned char field_18, field_19, field_1A, field_1B, field_1C, field_1D, field_1E, field_1F;
    unsigned char field_20, field_21, field_22, field_23, field_24, field_25;
} FloorRecord;
extern FloorRecord D_80142EF0;
extern unsigned char D_80147620[];
extern s32 func_800C587C(void *rng, unsigned char limit);
extern void *func_800A8640(s32 command, s32 flags);
void *func_800AA5F8(void) {
    void *result;
    if (func_800C587C(D_80147620, D_80142EF0.field_1B) == 0)
        result = 0;
    else
        result = func_800A8640(0x5A, 1);
    return result;
}
