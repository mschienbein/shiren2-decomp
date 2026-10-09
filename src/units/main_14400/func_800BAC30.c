#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;

/* Whole 0x26-byte floor record at D_80142EF0 (same layout as the canonical C views). */
typedef struct {
    unsigned char field_00, field_01, field_02, field_03, field_04, field_05, field_06, field_07;
    unsigned char field_08, field_09, field_0A, field_0B, field_0C, field_0D;
    unsigned short field_0E;
    unsigned char field_10, field_11, field_12, field_13, field_14, field_15, field_16, field_17;
    unsigned char field_18, field_19, field_1A, field_1B, field_1C, field_1D, field_1E, field_1F;
    unsigned char field_20, field_21, field_22, field_23, field_24, field_25;
} FloorRecord;

/* Partial view of the floor generator object. */
typedef struct {
    u8 pad0[0x400];
    s8 kind;
    u8 pad401;
    u16 field_402;
    u8 pad404[0x958 - 0x404];
    u16 flags;
} Floor;

extern FloorRecord D_80142EF0;
extern u8 D_80147620[];
extern s32 func_800C587C(void *rng, unsigned char chance);

/* Rolls the floor's feature flags from the per-floor chance table. */
void func_800BAC30(Floor *floor)
{
    FloorRecord *record = &D_80142EF0;

    floor->flags = 0;
    floor->field_402 = func_800C587C(D_80147620, record->field_0D) ? 0x80 : 0x2000;
    switch (floor->kind) {
    case 0:
        if (func_800C587C(D_80147620, record->field_0B)) {
            floor->flags |= 0x1;
        }
        if (func_800C587C(D_80147620, record->field_06)) {
            floor->flags |= 0x2;
        }
        if (func_800C587C(D_80147620, record->field_08)) {
            floor->flags |= 0x4;
        }
        if (func_800C587C(D_80147620, record->field_22)) {
            floor->flags |= 0x8;
        }
        if (func_800C587C(D_80147620, record->field_23)) {
            floor->flags |= 0x10;
        }
        if (func_800C587C(D_80147620, record->field_21)) {
            floor->flags |= 0x20;
        }
        if (func_800C587C(D_80147620, record->field_25)) {
            floor->flags |= 0x40;
        }
        if (func_800C587C(D_80147620, record->field_0A)) {
            floor->flags |= 0x80;
        }
        if (func_800C587C(D_80147620, record->field_1E)) {
            floor->flags |= 0x100;
        }
        if (func_800C587C(D_80147620, record->field_1D)) {
            floor->flags |= 0x200;
        }
        if (record->field_04 == 2) {
            floor->flags |= 0x400;
        }
        if (func_800C587C(D_80147620, record->field_24)) {
            floor->flags |= 0x800;
        }
        floor->flags |= 0x1000;
        break;
    case 13:
        if (func_800C587C(D_80147620, record->field_06)) {
            floor->flags |= 0x2;
        }
        if (func_800C587C(D_80147620, record->field_08)) {
            floor->flags |= 0x4;
        }
        if (func_800C587C(D_80147620, record->field_22)) {
            floor->flags |= 0x8;
        }
        if (func_800C587C(D_80147620, record->field_23)) {
            floor->flags |= 0x10;
        }
        if (func_800C587C(D_80147620, record->field_21)) {
            floor->flags |= 0x20;
        }
        if (func_800C587C(D_80147620, record->field_25)) {
            floor->flags |= 0x40;
        }
        if (func_800C587C(D_80147620, record->field_0A)) {
            floor->flags |= 0x80;
        }
        if (func_800C587C(D_80147620, record->field_1E)) {
            floor->flags |= 0x100;
        }
        if (record->field_04 == 2) {
            floor->flags |= 0x400;
        }
        if (func_800C587C(D_80147620, record->field_24)) {
            floor->flags |= 0x800;
        }
        floor->flags |= 0x1000;
        break;
    case 7:
    case 8:
    case 10:
        if (func_800C587C(D_80147620, record->field_06)) {
            floor->flags |= 0x2;
        }
        if (func_800C587C(D_80147620, record->field_08)) {
            floor->flags |= 0x4;
        }
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 9:
        if (func_800C587C(D_80147620, record->field_08)) {
            floor->flags |= 0x4;
        }
        break;
    case 1:  /* ODD_C: generator kinds 1, 11 and 12 (s8 +0x400, set by func_800BA6A0) */
    case 11: /* roll no feature flags; flags stays 0. These labels do not shape codegen: */
    case 12: /* without them the jump table sends 1/11/12 to the same switch exit. */
        break;
    }
}
