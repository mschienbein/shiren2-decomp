#include "common.h"

typedef unsigned char u8;
typedef struct { u8 *buf; u32 flags; } BufEntry;
typedef struct RenderContext RenderContext;
typedef struct { void *receive; void *send; long valid, first, capacity; void **messages; } Queue;
typedef struct { u8 counts[3]; u8 pad3; void *depthbuffer; BufEntry *entries; } BufPool;
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
extern u32 func_80031F90(u32 mask);

/* Releases the pool entry holding buf (clears its busy flag and the active
 * count) with interrupts masked. */
void func_8006B6F4(u8 *buf) {
    if (D_80190D80.pool.counts[2] != 0) {
        u32 mask = func_80031F90(1);
        u32 i;
        for (i = 0; i < D_80190D80.pool.counts[1]; i++) {
            if (D_80190D80.pool.entries[i].buf == buf && (D_80190D80.pool.entries[i].flags & 1)) {
                D_80190D80.pool.entries[i].flags &= ~1;
                D_80190D80.pool.counts[2]--;
                break;
            }
        }
        func_80031F90(mask);
    }
}
