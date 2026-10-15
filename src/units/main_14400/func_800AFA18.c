#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
extern const unsigned char D_8015488C[8];

static inline s32 bit_at(u8 *bits, s32 index) {
    s32 result = 0;
    if (bits[index >> 3] & D_8015488C[index & 7]) {
        result = 1;
    }
    return result;
}

/* Whether the reserved region [primaryCount, capacity) has a vacant record. */
s32 func_800AFA18(ItemPool *pool) {
    s32 index;
    s32 end;
    if (D_80143104) {
        return 1;
    }
    index = pool->primaryCount;
    end = pool->capacity;
    for (;;) {
        s32 set;
        if (index >= end) break;
        set = bit_at(pool->occupancy, index);
        if (set == 0) {
            return 1;
        }
        index++;
    }
    return 0;
}
