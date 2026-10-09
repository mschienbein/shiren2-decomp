#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
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
/* Whole 4-byte roll clamp at D_8015690C (next object D_80156910). */
typedef struct { u16 min; u16 max; } RollBounds;
extern FloorRecord D_80142EF0;
extern RollBounds D_8015690C;
extern u8 D_80147620[];
extern s32 func_800C5844(void *rng, u8 base, u8 top);

static inline u16 FloorRecord_rollPercent(FloorRecord *floor) { return floor->field_0E; }
static inline u16 RollBounds_min(RollBounds *bounds) { return bounds->min; }
static inline u16 RollBounds_max(RollBounds *bounds) { return bounds->max; }

u16 func_800AB044(void) {
    u8 roll = func_800C5844(D_80147620, 0, 200);
    u16 value = FloorRecord_rollPercent(&D_80142EF0) * roll / 100;
    if (value < RollBounds_min(&D_8015690C)) value = RollBounds_min(&D_8015690C);
    else if (value > RollBounds_max(&D_8015690C)) value = RollBounds_max(&D_8015690C);
    return value;
}
