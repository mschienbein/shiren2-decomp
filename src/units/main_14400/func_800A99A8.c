#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

s32 func_800A99A8(void) {
    return (u32)(D_80142F24.index - 12) < 8U;
}
