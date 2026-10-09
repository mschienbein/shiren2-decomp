#include "common.h"

/* Fixed 0x30-byte pool records (func_800B0164 stride). */
typedef struct { s32 words[0x30 / 4]; } PoolRecord;

/* Pool header built by func_800AF890: record storage at +0, occupancy bitset at +4,
 * capacity at +8 and free count at +0xC (func_800AF910). */
typedef struct {
    PoolRecord *base_00;
    unsigned char *bits_04;
    s32 count_08;
    s32 free_0C;
} Collection;

/* Pool-exhaustion sentinel, initialized to the address D_801C5670. */
extern PoolRecord *D_80143090;

s32 func_800AFD08(Collection *collection, PoolRecord *record) {
    u32 offset;
    u32 index;
    if (record == 0 || record == D_80143090) {
        return 0xFF;
    }
    /* local-arithmetic-qualification: `record` is not known to point into this pool's
     * storage, so pointer subtraction would be undefined. The original rejects foreign
     * or misaligned records by comparing the 32-bit target addresses (subu at 0x800AFD2C,
     * unsigned divide-by-48 and remainder check through 0x800AFD50); only this local
     * calculation uses integers, the interface and the stored fields stay pointers. */
    offset = (u32)record - (u32)collection->base_00;
    index = offset / 48U;
    if (offset != index * 48U) {
        return 0xFF;
    }
    offset = index;
    if ((s32)offset < 0 || (s32)offset >= collection->count_08) {
        return 0xFF;
    }
    return offset & 0xFF;
}
