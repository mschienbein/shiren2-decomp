#include "common.h"
#include "map_position_iterator.h"
typedef struct { char pad[0x400]; signed char field_400; unsigned short field_402; } Obj;
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
extern char D_80147620[];
extern MapRect D_801429C0;
extern s32 func_800C587C(void *, unsigned char);
extern u32 func_800B1C6C(MapPos *pos);
extern void func_800B1AE0(MapPos *pos, unsigned short value);
static inline MapPos *setPoint(MapPos *p, s32 x, s32 y) { p->x = x; p->y = y; return p; }
static inline void initIterator(MapPositionIterator *p, MapPos *temp, s32 x0, s32 y0, s32 x1, s32 y1) {
    p->start = *setPoint(temp, x0, y0); p->cur = p->start; p->end = *setPoint(temp, x1, y1);
}
void func_800BC580(Obj *a) {
    MapPositionIterator iter; MapPos temp; MapPositionIterator *it; MapPos *p;
    if (a->field_400 == 1) return;
    if (a->field_400 == 2) return;
    if (a->field_400 == 3) return;
    if (a->field_402 == 0x80) return;
    { s32 failed = func_800C587C(D_80147620, D_80142EF0.field_0C) != 1; if (failed) return; }
    it = &iter;
    initIterator(it, &temp, D_801429C0.min.x, D_801429C0.min.y, D_801429C0.max.x, D_801429C0.max.y);
    p = &temp;
    for (;;) { s32 valid = iter.cur.x <= it->end.x; if (!valid) break;
        func_800A3610(p, it);
        if (func_800B1C6C(p) & 0x4000) func_800B1AE0(p, 0x2000);
    }
}
