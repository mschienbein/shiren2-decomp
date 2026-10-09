#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;


extern s32 func_800A99A8(void);
extern void *func_800AADB4(unsigned char id, s32 value);

void *func_800AB134(void) {
    if ((func_800A99A8() ^ 1) == 0) {
        return func_800AADB4(D_80142F24.field_08, 0);
    }
    return 0;
}
