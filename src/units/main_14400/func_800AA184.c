#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

typedef unsigned char u8;

typedef struct { u8 lo, hi, min, max, field_4, kind, chance, field_7; } Range;



typedef struct Rng800AA184 Rng800AA184;

extern const Range D_80156CD0[8];
extern Rng800AA184 D_80147620;


extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern void *func_800ABFE4(s32 index);
extern u8 func_800AB1C8(void *table, u8 a, s32 b);
extern void func_800A9C8C(u8 a, u8 b);

s32 func_800AA184(u8 id, u8 arg) {
    const Range *entry;

    if ((u8)(id - 0xC) < 8) {
        entry = &D_80156CD0[id - 12];
        D_80142F24.previous_count = func_800C5844(&D_80147620, entry->min, entry->max) - 1;
        D_80142F24.field_06 = arg;
        D_80142F24.field_08 = func_800AB1C8(func_800ABFE4((u8)(entry->field_7 - 1)), 0, 0);
        func_800A9C8C(id, 0);
        return 1;
    }
    return 0;
}
