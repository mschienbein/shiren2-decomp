#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

typedef unsigned char u8;
typedef unsigned short u16;


extern u16 D_801476C0;

void func_800A9A90(void)
{
    SelectionSave *state = &D_80142F24;

    if (state->index == 0xB && state->count < 3) {
        D_801476C0 = state->count + 0xFA1;
    }
}
