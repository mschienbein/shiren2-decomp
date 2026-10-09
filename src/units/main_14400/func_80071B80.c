#include "common.h"

typedef struct { unsigned char active; unsigned char unk01[0x27]; } Record;
typedef struct {
    unsigned char count;
    unsigned char unk01[0x13];
    Record *records;
} Group;
extern u32 D_8013D518;
extern Group *D_8013D51C;
typedef unsigned char u8;
typedef struct CacheEntry CacheEntry;
/* Whole 0x2C-byte sprite cache at D_801A7190: func_800718CC fills capacity +0x0, stride +0x4,
 * palette stride +0x8 and the allocated data +0xC, palette +0x10 and bank +0x14/+0x18 buffers. */
typedef struct {
    u8 capacity;
    u8 pad01[3];
    s32 stride;
    u32 palStride;
    u8 *data;
    u8 *palBase;
    CacheEntry *banks[2];
    u8 bank;
    u8 pad1D[3];
    u32 count;
    u8 *cursor;
    u8 *limit;
} SpriteCache;
extern SpriteCache D_801A7190;

void func_80071B80(void) {
    u32 group_index;
    s32 item_index;
    Group *group;
    unsigned char *start;
    for (group_index = 0; group_index < D_8013D518; group_index++) {
        group = &D_8013D51C[group_index];
        for (item_index = 0; item_index < group->count; item_index++) {
            group->records[item_index].active = 0;
        }
    }
    D_801A7190.bank = (D_801A7190.bank + 1) & 1;
    D_801A7190.count = 0;
    start = D_801A7190.data + D_801A7190.stride * D_801A7190.capacity * D_801A7190.bank;
    D_801A7190.cursor = start;
    D_801A7190.limit = start + D_801A7190.stride * D_801A7190.capacity;
}
