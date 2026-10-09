#include "common.h"

typedef unsigned char u8;
/* Opaque view of the 0x18-byte sprite pool records (modelled in func_80071CCC). */
typedef struct SpritePool SpritePool;
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
extern s32 D_8013D518;
extern SpritePool *D_8013D51C;
extern SpriteCache D_801A7190;

void func_80071B60(void) {
    D_8013D518 = 0;
    D_8013D51C = 0;
    D_801A7190.capacity = 0;
}
