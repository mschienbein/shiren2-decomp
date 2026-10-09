#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;
typedef unsigned char u8;

char *func_800A9720(u8 kind, u8 variant, u8 flags);
char *func_800A9864(void)
{
    SelectionSave *state = &D_80142F24;
    return func_800A9720(state->index, state->count, 1);
}
