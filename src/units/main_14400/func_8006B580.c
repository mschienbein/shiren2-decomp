#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 *buf;
    u32 flags;
} BufEntry;

typedef struct RenderContext RenderContext;
typedef struct { void *receive; void *send; long valid, first, capacity; void **messages; } Queue;
typedef struct {
    u8 counts[3]; /* cursor, capacity, active */
    u8 pad3;
    void *depthbuffer;
    BufEntry *entries;
} BufPool;
/* The renderer thread receives 0x80190D80; queue +0x368 and pool +0x3A0
 * belong to it. Padding covers the scheduler pointer and two SDK threads. */
typedef struct {
    u8 pad00[0x368];
    Queue queue;
    void *messages[8];
    BufPool pool;
    s32 (*first_callback)(RenderContext *);
    s32 (*second_callback)(RenderContext *);
    u8 phase;
    u8 pad3B5[3];
} RenderManager;
extern RenderManager D_80190D80;

u32 func_80031F90(u32 mask);
void func_80033048(const char *fmt, ...);

/* Lock the pool buffer `offset` slots after the cursor; NULL if none is free. */
u8 *func_8006B580(s32 offset)
{
    s32 index = (D_80190D80.pool.counts[0] + (u8)offset) % D_80190D80.pool.counts[1];
    u32 mask;

    if (D_80190D80.pool.counts[2] >= D_80190D80.pool.counts[1] - 1 || (D_80190D80.pool.entries[index].flags & 1)) {
        func_80033048("OOGM : FrameBufferLock Failed !!\n");
        return 0;
    }
    mask = func_80031F90(1);
    D_80190D80.pool.counts[2]++;
    D_80190D80.pool.entries[index].flags |= 1;
    while (D_80190D80.pool.entries[D_80190D80.pool.counts[0]].flags & 1) {
        D_80190D80.pool.counts[0] = (D_80190D80.pool.counts[0] + 1) % D_80190D80.pool.counts[1];
    }
    func_80031F90(mask);
    return D_80190D80.pool.entries[index].buf;
}
