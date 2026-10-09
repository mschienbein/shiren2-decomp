#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;


s32 func_800A99D0(void) {
    if ((D_80142F18.flags >> 2) & 1) return 1;
    return D_80142F24.index == 9 || D_80142F24.index == 10 || D_80142F24.index == 20;
}
