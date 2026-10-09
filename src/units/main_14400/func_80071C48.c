#include "common.h"

typedef unsigned char u8;

/* View of func_80071CCC's 0x2C-byte CacheEntry. */
typedef struct { u8 pad0[0x24]; u8 *data; u8 pad28[4]; } CacheEntry80071C48;
/* Whole 0x2C-byte sprite cache at D_801A7190: func_800718CC fills capacity +0x0, stride +0x4,
 * palette stride +0x8 and the allocated data +0xC, palette +0x10 and bank +0x14/+0x18 buffers. */
typedef struct {
    u8 capacity;
    u8 pad01[3];
    s32 stride;
    u32 palStride;
    u8 *data;
    u8 *palBase;
    CacheEntry80071C48 *banks[2];
    u8 bank;
    u8 pad1D[3];
    u32 count;
    u8 *cursor;
    u8 *limit;
} SpriteCache80071C48;
extern SpriteCache80071C48 D_801A7190;
/* Clears the data pointer of each entry past count whose data lies below the cache cursor. */
void func_80071C48(void) {
    s32 i;
    for (i = D_801A7190.count; i < D_801A7190.capacity; i++) {
        CacheEntry80071C48 *e = &D_801A7190.banks[D_801A7190.bank][i];
        if (e->data != 0 && e->data < D_801A7190.cursor) {
            e->data = 0;
        }
    }
}
