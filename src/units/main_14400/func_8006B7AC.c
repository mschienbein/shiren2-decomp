#include "common.h"
typedef unsigned char u8;
typedef struct RenderContext RenderContext;
typedef struct { void *receive; void *send; long valid, first, capacity; void **messages; } Queue;
typedef struct { u8 *buf; u32 flags; } BufEntry;
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
extern s32 func_8006D184(u8 id, u8 on);
extern long func_8002FEA0(Queue *queue, void **message, long flags);
void func_8006B7AC(void) {
    func_8006D184(3, 1);
    D_80190D80.phase = 1;
    func_8002FEA0(&D_80190D80.queue, 0, 1);
    func_8006D184(3, 0);
}
