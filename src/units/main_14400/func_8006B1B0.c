#include "common.h"

typedef struct RenderContext RenderContext;
extern void func_8006CD14(void);
typedef unsigned char u8;
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

void func_8006B1B0(s32 (*callback)(RenderContext *)) {
    func_8006CD14();
    D_80190D80.second_callback = callback;
}
