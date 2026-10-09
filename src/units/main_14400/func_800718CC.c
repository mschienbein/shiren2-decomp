#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
/* Layouts shared with func_80071CCC (sprite pool records and the sprite cache). */
typedef struct { u8 flags; u8 pad1[3]; u32 pals; u8 frameCount; u8 pad9[3]; u32 frames; } SpriteHeader;
typedef union { u32 segmented; void *resident; } FrameData;
typedef struct { FrameData data; u8 pad4[2]; u8 width; u8 height; u32 pad8; } SpriteFrame;
typedef struct {
    u16 id;
    u8 pad2[2];
    SpriteHeader header;
    u8 frameTag;
    u8 pad15[3];
    SpriteFrame frame;
    u8 *data;
    u8 palTag;
    u8 pad29[3];
} CacheEntry;
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
typedef struct {
    u8 busy;
    u8 pad1;
    u16 id;
    SpriteHeader header;
    u8 frameBuf;
    u8 frameTag;
    u8 pad16[2];
    SpriteFrame frame;
    u8 palBuf;
    u8 palTag;
    u8 pad26[2];
} PoolSlot;
typedef struct {
    u8 count;
    u8 pad1[3];
    u32 stride;
    u32 palStride;
    u8 *base;
    u8 *palBase;
    PoolSlot *slots;
} SpritePool;
/* Caller-supplied 12-byte pool description: slot count at +3, data and palette strides. */
typedef struct {
    u8 pad0[3];
    u8 count;
    u32 stride;
    u32 palStride;
} PoolSpec;

extern u32 D_8013D518;
extern SpritePool *D_8013D51C;
extern SpriteCache D_801A7190;
extern const char D_8014C900[], D_8014C914[], D_8014C928[], D_8014C940[];
extern void *func_8006A8D8(char *name, u32 size);
extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);

s32 func_800718CC(u32 count, PoolSpec *specs, PoolSpec *cacheSpec) {
    PoolSpec *spec = cacheSpec;
    s32 result = 0;
    u32 index;
    SpriteCache *cache;
    s32 size;

    if (count >= 255) {
        result = -1;
    } else {
        D_8013D518 = count;
        if (count != 0) {
            D_8013D51C = func_8006A8D8((char *)D_8014C900, count * sizeof(SpritePool));
            for (index = 0; index < D_8013D518; index++) {
                D_8013D51C[index].base = 0;
                D_8013D51C[index].palBase = 0;
                D_8013D51C[index].count = specs[index].count;
                D_8013D51C[index].stride = specs[index].stride;
                D_8013D51C[index].palStride = specs[index].palStride;
            }
            for (index = 0; index < D_8013D518; index++) {
                SpritePool *pool = &D_8013D51C[index];
                if (pool->count != 0 && pool->stride != 0) {
                    size = pool->stride * (pool->count << 1);
                    pool->base = func_8006A8D8((char *)D_8014C900, size);
                    size = pool->palStride * (pool->count << 1);
                    pool->palBase = func_8006A8D8((char *)D_8014C914, size);
                    size = pool->count * sizeof(PoolSlot);
                    pool->slots = func_8006A8D8((char *)D_8014C928, size);
                    func_8006A810((u8 *)pool->slots, 0, size);
                }
            }
        }
        cache = &D_801A7190;
        if (spec != 0) {
            s32 i;
            cache->capacity = spec->count;
            cache->stride = spec->stride;
            cache->palStride = spec->palStride;
            cache->data = func_8006A8D8((char *)D_8014C940, cache->stride * (cache->capacity << 1));
            cache->palBase = func_8006A8D8((char *)D_8014C940, cache->palStride * (cache->capacity << 1));
            size = cache->capacity * sizeof(CacheEntry);
            for (i = 0; i < 2; i++) {
                cache->banks[i] = func_8006A8D8((char *)D_8014C928, size);
                func_8006A810((u8 *)cache->banks[i], 0, size);
            }
        } else {
            cache->capacity = 0;
        }
    }
    return result;
}
