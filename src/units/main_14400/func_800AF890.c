#include "common.h"
typedef unsigned char u8;
/* 0x10-byte pool header (D_80143094.. are spaced 0x10 apart): record storage, the
 * occupancy bit buffer cleared by func_800AF8D0, the capacity, and the free count
 * that func_800AF910 sets to capacity minus its argument. */
typedef struct {
    void *storage;
    u8 *bits;
    s32 capacity;
    s32 free;
} Pool;
void func_800AF910(Pool *pool, s32 used);
void func_800AF8D0(Pool *pool);
Pool *func_800AF890(Pool *pool, void *storage, u8 *bits, s32 capacity) {
    pool->storage = storage;
    pool->bits = bits;
    pool->capacity = capacity;
    func_800AF910(pool, 0);
    func_800AF8D0(pool);
    return pool;
}
