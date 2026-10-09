#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;


void func_800A94F0(void) {
    SelectionSave *p = &D_80142F24;
    p->field_03 = 1;
    p->index = 0;
    p->count = 0;
    p->previous = 0;
    p->masks[0] = 0;
    p->masks[1] = 0;
    p->result = -1;
}
