#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;


extern unsigned char D_80156CA0[];

unsigned char func_800AA928(void) {
    return D_80156CA0[D_80142F24.index];
}
