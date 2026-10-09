#include "common.h"
typedef unsigned char u8;
typedef struct { u8 bytes[0x30]; } Slot;
typedef struct { Slot *slots; u8 *used; s32 capacity; s32 count; } Pool;
extern Slot *D_80143104;
extern Slot D_801C5670[];
Slot *D_80143090 = D_801C5670;
extern const u8 D_8015488C[8];
Slot *func_800AFA7C(Pool *pool) {
    s32 index = 0;
    if (D_80143104) return D_80143104;
    for (;;) {
        u8 *byte;
        u8 used, bit;
        if (!(index < pool->count)) break;
        byte = &pool->used[index >> 3];
        used = *byte;
        bit = D_8015488C[index & 7];
        if (!(used & bit)) {
            *byte = used | bit;
            return &pool->slots[index];
        }
        index++;
    }
    return D_80143090;
}
Slot *func_800AFAFC(Pool *pool) {
    s32 index;
    if (D_80143104) return D_80143104;
    index = pool->count;
    for (;;) {
        u8 *byte;
        u8 used, bit;
        if (!(index < pool->capacity)) break;
        byte = &pool->used[index >> 3];
        used = *byte;
        bit = D_8015488C[index & 7];
        if (!(used & bit)) {
            *byte = used | bit;
            return &pool->slots[index];
        }
        index++;
    }
    return D_80143090;
}
