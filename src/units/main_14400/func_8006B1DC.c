#include "common.h"
typedef unsigned char u8;
typedef void *ProbeMessage;
typedef struct { void *receive, *send; s32 valid, first, count; ProbeMessage *messages; } ProbeMessageQueue;
typedef struct Node8006C350 { struct Node8006C350 *next; void *mq; u8 flags; } Node8006C350;
typedef struct { void *image; u32 flags; } Frame;
typedef struct RenderContext { u8 frame, job; Frame *buffer; void *work; } RenderContext;
typedef struct { u8 pad0[0x9F8]; u8 frames, jobs, busy; } Scheduler;
typedef struct {
    Scheduler *scheduler; u8 pad4[0x364]; ProbeMessageQueue completed;
    u8 pad380[0x20]; u8 current, count; u8 pad3A2[2]; void *work;
    Frame *buffers; s32 (*produce)(RenderContext *); s32 (*consume)(RenderContext *); u8 phase;
} Worker;
extern const char D_8014C630[], D_8014C650[];
extern void func_80027EA0(ProbeMessageQueue *queue, ProbeMessage *messages, long capacity);
extern long func_8002FEA0(ProbeMessageQueue *queue, ProbeMessage *message, long flags);
extern long func_80031D50(ProbeMessageQueue *queue, ProbeMessage message, long flags);
extern void func_80033048(const char *fmt, ...);
extern s32 func_8006B4E4(void);
extern void func_8006C350(Node8006C350 *node, void *mq, u8 flags);
extern s32 func_8006D184(u8 id, u8 on);
void func_8006B1DC(Worker *worker) {
    RenderContext context;
    Node8006C350 node;
    ProbeMessageQueue queue;
    ProbeMessage messages[8];
    ProbeMessage message;
    func_80033048(D_8014C630);
    func_80027EA0(&queue, messages, 8);
    func_8006C350(&node, &queue, 3);
    for (;;) {
        u8 phase;
        u8 frames;
        func_8002FEA0(&queue, &message, 1);
        if (*(s32 *)message) continue;
        phase = worker->phase;
        if (phase != 1) continue;
        frames = worker->scheduler->frames;
        if (frames >= func_8006B4E4() - 1) continue;
        worker->scheduler->frames++;
        worker->scheduler->busy = phase;
        if (worker->produce) {
            worker->scheduler->jobs++;
            context.job = worker->scheduler->jobs;
            context.frame = worker->scheduler->frames;
            context.buffer = &worker->buffers[worker->current];
            context.work = worker->work;
            func_8006D184(4, 0);
            if (worker->produce(&context)) {
                worker->scheduler->jobs--;
                worker->scheduler->frames--;
                worker->scheduler->busy = 0;
                continue;
            }
            func_8006D184(4, 1);
        }
        worker->phase = 2;
    }
}
/* Separately installed consumer thread (func_80027ED0 entry at 0x8006B0D0); own catalogue entry. */
void func_8006B334(Worker *worker) {
    RenderContext context;
    Node8006C350 node;
    ProbeMessageQueue queue;
    ProbeMessage messages[8];
    ProbeMessage message;
    func_80033048(D_8014C650);
    func_80027EA0(&queue, messages, 8);
    func_8006C350(&node, &queue, 3);
    for (;;) {
        func_8002FEA0(&queue, &message, 1);
        if (*(s32 *)message || worker->phase != 2) continue;
        if (worker->consume) {
            worker->scheduler->jobs++;
            context.job = worker->scheduler->jobs;
            context.frame = worker->scheduler->frames;
            context.buffer = &worker->buffers[worker->current];
            context.work = worker->work;
            func_8006D184(4, 0);
            if (worker->consume(&context)) {
                worker->scheduler->jobs--;
                worker->scheduler->frames--;
                worker->scheduler->busy = 0;
                worker->phase = 1;
                continue;
            }
            func_8006D184(4, 1);
        } else worker->scheduler->frames--;
        worker->scheduler->busy = 0;
        worker->phase = 0;
        do { worker->current = (worker->current + 1) % worker->count; }
        while (worker->buffers[worker->current].flags & 1);
        func_80031D50(&worker->completed, 0, 1);
    }
}
