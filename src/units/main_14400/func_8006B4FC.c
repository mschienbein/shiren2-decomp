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

/* Mark the pool entry for buf (flag 2) with interrupts masked. */
void func_8006B4FC(u8 *buf) {
    u32 saved = func_80031F90(1);
    u32 i;

    for (i = 0; i < D_80190D80.pool.counts[1]; i++) {
        if (D_80190D80.pool.entries[i].buf == buf) {
            D_80190D80.pool.entries[i].flags |= 2;
            break;
        }
    }
    func_80031F90(saved);
}
