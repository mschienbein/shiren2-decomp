#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;

typedef struct {
    unsigned char data[11];
} Entry11;


extern Entry11 D_80156BA0[];

Entry11 *func_800AA900(void) {
    return &D_80156BA0[D_80142F24.index];
}
