#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;
typedef unsigned char u8;
typedef struct { u8 lo, hi, min, max, field_4, kind, chance, field_7; } Range;
extern const Range D_80156CD0[8];


s32 func_800AA24C(void) {
    s32 i;
    i = 8;
    for (;;) {
        const Range *r;
        i--;
        if (i == -1) break;
        r = &D_80156CD0[i];
        if (r->kind != D_80142F24.index) continue;
        if (r->lo > D_80142F18.kind) continue;
        if (D_80142F18.kind > r->hi) continue;
        return i + 12;
    }
    return 0;
}
