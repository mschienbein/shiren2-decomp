#include "common.h"
/* Pool header: record storage, occupancy bits, capacity, free count (func_800AF890). */
typedef struct PoolRecord PoolRecord;
typedef struct { PoolRecord *field_0; unsigned char *field_4; s32 field_8; s32 field_C; } Bits;
/* Record published by func_800B0164 while a pool is being restored; null otherwise. */
extern PoolRecord *D_80143104;
extern const unsigned char D_8015488C[8];
s32 func_800AF950(void *arg) {
    Bits *bits = arg;
    s32 index;
    if (D_80143104) return 1;
    index = 0;
    while (1) {
        if (index >= bits->field_C) break;
        if (!(bits->field_4[index >> 3] & D_8015488C[index & 7])) return 1;
        index++;
    }
    return 0;
}
