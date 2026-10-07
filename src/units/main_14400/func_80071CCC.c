#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u32 id; u32 frame; u8 pal; u8 kind; u8 slot; } SpriteReq;
typedef struct { u8 flags; u8 pad1[3]; u32 pals; u8 frameCount; u8 pad9[3]; u32 frames; } SpriteHeader;
/* A frame holds an encoded ROM address before loading, or a RAM pointer in a resident descriptor. */
typedef union { u32 segmented; void *resident; } FrameData;
typedef struct { FrameData data; u8 pad4[2]; u8 width; u8 height; u32 pad8; } SpriteFrame;
typedef struct { u8 flags; u8 pad1[3]; void **pals; u8 frameCount; u8 pad9[3]; SpriteFrame *frames; } SpriteDesc;
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
    u8 pad1[7];
    u32 palStride;
    u32 pad0C;
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
extern s32 D_8013D514;
extern u32 D_8013D518;
extern SpritePool *D_8013D51C;
extern SpriteDesc *D_801D25A0[];
extern SpriteCache D_801A7190;
extern char D_5000000[];
extern char D_00E53DB0[];
void func_8006AAF0(void *, u32, s32);
s32 func_800724DC(SpriteFrame *, void *);
s32 func_8007248C(u32, void *, s32);
s32 func_80071CCC(SpriteReq *req, void **outData, void **outPal, SpriteHeader **outHeader, SpriteFrame **outFrame) {
    u32 buf[4];
    s32 ret = 0;
    u8 *data = 0;
    u8 *palAddr = 0;
    SpriteHeader *header = 0;
    SpriteFrame *frame = 0;
    SpriteDesc *desc;
    u32 size;
    u32 palOff;
    u32 palCount;
    do {
        if (D_8013D514 == 0) {
            ret = -1;
            break;
        }
        if (req->id >= 0x195) {
            ret = -1;
            break;
        }
        desc = D_801D25A0[req->id];
        if (desc != 0) {
            header = (SpriteHeader *)desc;
            palCount = desc->flags & 0x1F;
            if (req->frame >= header->frameCount) req->frame = header->frameCount - 1;
            if (req->pal >= palCount) req->pal = palCount - 1;
            frame = &desc->frames[req->frame];
            data = frame->data.resident;
            palAddr = desc->pals[req->pal];
            break;
        }
        if (req->kind == 0xFE) {
            SpriteCache *cache = &D_801A7190;
            CacheEntry *e;
            u32 i;
            if (cache->count >= cache->capacity) {
                ret = -1;
                break;
            }
            for (i = 0; i < cache->count; i++) {
                e = &cache->banks[cache->bank][i];
                if (req->id == e->id && req->frame == e->frameTag && req->pal == e->palTag) {
                    header = &e->header;
                    frame = &e->frame;
                    data = e->data;
                    palAddr = cache->palBase + cache->palStride * (i + cache->capacity * cache->bank);
                    break;
                }
            }
            if (i < cache->count) break;
            e = &cache->banks[cache->bank][cache->count];
            if (cache->cursor != e->data) e->frameTag = 0xFF;
            if (e->id == 0 || e->id != req->id) {
                func_8006AAF0(buf, (((u32)D_5000000 & 0xFFFFFF) + (u32)D_00E53DB0) + req->id * 16, 0x10);
                e->header = *(SpriteHeader *)buf;
                e->id = req->id;
                e->frameTag = 0xFF;
                e->palTag = 0xFF;
            }
            header = &e->header;
            frame = &e->frame;
            palCount = header->flags & 0x1F;
            if (req->frame >= header->frameCount) req->frame = header->frameCount - 1;
            if (req->pal >= palCount) req->pal = palCount - 1;
            if (e->frameTag != req->frame) {
                func_8006AAF0(buf, ((header->frames & 0xFFFFFF) + (u32)D_00E53DB0) + req->frame * 12, 0xC);
                e->frame = *(SpriteFrame *)buf;
                e->frameTag = 0xFF;
            }
            size = (((s32)(frame->width * (frame->height + 1) + 1) >> 1) + 7) & ~7;
            data = cache->cursor;
            if (e->frameTag != req->frame) {
                if (data + size > cache->limit) {
                    ret = -1;
                    break;
                }
                if (func_800724DC(frame, data)) {
                    ret = -1;
                    break;
                }
                e->frameTag = req->frame;
                e->data = data;
            }
            if (e->palTag != req->pal) {
                func_8006AAF0(buf, ((header->pals & 0xFFFFFF) + (u32)D_00E53DB0) + req->pal * 4, 4);
                palOff = buf[0];
            }
            palAddr = cache->palBase + cache->palStride * (cache->count + cache->capacity * cache->bank);
            if (e->palTag != req->pal) {
                if (cache->palStride < 0x20) {
                    ret = -1;
                    break;
                }
                if (func_8007248C(palOff, palAddr, 0x20)) {
                    ret = -1;
                    break;
                }
                e->palTag = req->pal;
            }
            cache->count++;
            cache->cursor += size;
            break;
        }
        if (req->kind >= D_8013D518) {
            ret = -1;
            break;
        }
        {
            SpritePool *pool = &D_8013D51C[req->kind];
            PoolSlot *s;
            if (req->slot >= pool->count) {
                ret = -1;
                break;
            }
            s = &pool->slots[req->slot];
            if (s->busy != 0) {
                ret = -1;
                break;
            }
            if (s->id == 0 || s->id != req->id) {
                func_8006AAF0(buf, (((u32)D_5000000 & 0xFFFFFF) + (u32)D_00E53DB0) + req->id * 16, 0x10);
                s->header = *(SpriteHeader *)buf;
                s->id = req->id;
                s->frameTag = 0xFF;
                s->palTag = 0xFF;
            }
            header = &s->header;
            frame = &s->frame;
            palCount = header->flags & 0x1F;
            if (req->frame >= header->frameCount) req->frame = header->frameCount - 1;
            if (req->pal >= palCount) req->pal = palCount - 1;
            if (s->frameTag != req->frame) {
                s->frameBuf = (s->frameBuf + 1) & 1;
                func_8006AAF0(buf, ((header->frames & 0xFFFFFF) + (u32)D_00E53DB0) + req->frame * 12, 0xC);
                s->frame = *(SpriteFrame *)buf;
                s->frameTag = 0xFF;
            }
            data = pool->base + pool->stride * (req->slot + pool->count * s->frameBuf);
            size = (((s32)(frame->width * (frame->height + 1) + 1) >> 1) + 7) & ~7;
            if (s->frameTag != req->frame) {
                if (pool->stride < size) {
                    ret = -1;
                    break;
                }
                if (func_800724DC(frame, data)) {
                    ret = -1;
                    break;
                }
                s->frameTag = req->frame;
            }
            if (s->palTag != req->pal) {
                s->palBuf = (s->palBuf + 1) & 1;
                func_8006AAF0(buf, ((header->pals & 0xFFFFFF) + (u32)D_00E53DB0) + req->pal * 4, 4);
                palOff = buf[0];
            }
            palAddr = pool->palBase + pool->palStride * (req->slot + pool->count * s->palBuf);
            if (s->palTag != req->pal) {
                if (pool->palStride < 0x20) {
                    ret = -1;
                    break;
                }
                if (func_8007248C(palOff, palAddr, 0x20)) {
                    ret = -1;
                    break;
                }
                s->palTag = req->pal;
            }
            s->busy = 1;
        }
    } while (0);
    if (ret == 0) {
        *outData = data;
        *outPal = palAddr;
        *outHeader = header;
        *outFrame = frame;
    }
    return ret;
}
