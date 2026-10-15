#include "common.h"
#include "item_records.h"
typedef unsigned char u8;
/* Sentinel record immediately below pool D_80143094's storage (D_801C56A0). */
extern Item D_801C5670[];
Item *D_80143090 = D_801C5670;
extern const u8 D_8015488C[8];
Item *func_800AFA7C(ItemPool *pool) {
    s32 index = 0;
    if (D_80143104) return D_80143104;
    for (;;) {
        u8 *byte;
        u8 used, bit;
        if (!(index < pool->primaryCount)) break;
        byte = &pool->occupancy[index >> 3];
        used = *byte;
        bit = D_8015488C[index & 7];
        if (!(used & bit)) {
            *byte = used | bit;
            return &pool->items[index];
        }
        index++;
    }
    return D_80143090;
}
Item *func_800AFAFC(ItemPool *pool) {
    s32 index;
    if (D_80143104) return D_80143104;
    index = pool->primaryCount;
    for (;;) {
        u8 *byte;
        u8 used, bit;
        if (!(index < pool->capacity)) break;
        byte = &pool->occupancy[index >> 3];
        used = *byte;
        bit = D_8015488C[index & 7];
        if (!(used & bit)) {
            *byte = used | bit;
            return &pool->items[index];
        }
        index++;
    }
    return D_80143090;
}
