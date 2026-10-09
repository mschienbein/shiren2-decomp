#include "common.h"

typedef unsigned char u8;
typedef struct RenderContext RenderContext;
typedef struct { u8 pad0[0x9EC]; s32 (*callback)(void); s32 retraces; u8 rate, divisor, flags[6]; u8 tail9FC[4]; } Scheduler;
typedef struct { unsigned long long storage[0x36]; } Thread;
typedef struct { void *receive, *send; long valid, first, capacity; void **messages; } Queue;
typedef struct { void *image; u32 flags; } Buffer;
typedef struct {
    Scheduler *scheduler;
    s32 frame;
    Thread producer, consumer;
    Queue queue;
    void *messages[8];
    u8 cursor, count, active, pad3A3;
    void *depthbuffer;
    Buffer *buffers;
    s32 (*produce)(RenderContext *);
    s32 (*consume)(RenderContext *);
    u8 phase;
    u8 pad3B5[3];
} Manager;
typedef struct { u32 command, value; } Gfx;
typedef struct {
    u32 type, flags;
    void *boot;
    u32 boot_size;
    void *ucode;
    u32 ucode_size;
    void *ucode_data;
    u32 ucode_data_size;
    void *stack;
    u32 stack_size;
    void *output, *output_end, *data;
    u32 data_size;
    void *yield;
    u32 yield_size;
} Task;
typedef struct {
    u8 state, kind, pad02[2];
    void *framebuffer, *depthbuffer;
    u8 pad0C[4];
    Task task;
    void *reply_queue, *reply_message;
} TaskRecord;
extern Manager D_80190D80;
extern Scheduler D_801D85D0;
extern u8 D_801998B8;
extern u8 D_80136A10[0xD0], D_8018FD80[0x400], D_8016FD80[0x20000], D_80190180[0xC00];
extern u8 D_80136AE0[];
/* Producer stack: its base had no standalone label in the assembly catalogue. */
extern u8 D_80191138[0x4000];
extern u8 D_80195138[0x4000];
extern u8 D_13CA50[];
extern TaskRecord D_80199138[];
extern s32 func_8006B4E4(void);
extern void func_8006B1DC(void *), D_8006B334(void *);
extern void func_8006B910(s32, s32, s32, s32);
extern void func_80027EA0(Queue *, void **, long);
extern void func_80027ED0(Thread *, s32, void (*)(void *), void *, void *, s32);
extern void func_80032B50(Thread *);
extern void func_8006B7F8(Gfx *, s32, s32, s32, void *, void *, void *, void *);
extern void func_8006CC80(void);
/* ODD_C: The whole-manager accessor keeps the buffer-table address reload
 * separate from the state pointer used for the other submitted fields. */
static inline Buffer *manager_buffers(Manager *state) { return state->buffers; }
static inline Gfx *emit(Gfx *cursor, u32 command, u32 value) {
    cursor->command = command;
    cursor->value = value;
    return cursor + 1;
}

void func_8006AEB0(s32 count, void *buffers, void *depthbuffer) {
    Gfx commands[256];
    Gfx *end;
    Queue *queue;
    Thread *thread1, *thread2;
    Manager *state;
    u32 i;
    D_80190D80.scheduler = &D_801D85D0;
    D_801998B8 = 0;
    D_80190D80.scheduler->callback = func_8006B4E4;
    D_80190D80.count = count;
    D_80190D80.buffers = buffers;
    D_80190D80.depthbuffer = depthbuffer;
    D_80190D80.produce = 0;
    D_80190D80.consume = 0;
    D_80190D80.phase = 0;
    D_80190D80.cursor = 0;
    D_80190D80.active = 0;
    func_8006B910(-1, -1, -1, -1);
    for (i = 0; i < D_80190D80.count; i++) D_80190D80.buffers[i].flags = 2;
    for (i = 0; i < 20; i++) {
        D_80199138[i].task.type = 1;
        D_80199138[i].task.flags = 0;
        D_80199138[i].task.boot = D_80136A10;
        /* local-arithmetic-qualification: the linked boot-image boundaries
         * define its byte extent; sizeof folds away the original subtraction. */
        D_80199138[i].task.boot_size = (u32)D_80136AE0 - (u32)D_80136A10;
        D_80199138[i].task.ucode_size = 0x1000;
        D_80199138[i].task.ucode_data_size = 0x800;
        D_80199138[i].task.stack = D_8018FD80;
        D_80199138[i].task.stack_size = 0x400;
        D_80199138[i].task.output = D_8016FD80;
        D_80199138[i].task.output_end = D_8016FD80 + sizeof(D_8016FD80);
        D_80199138[i].task.yield = D_80190180;
        D_80199138[i].task.yield_size = 0xC00;
    }
    queue = &D_80190D80.queue;
    func_80027EA0(queue, D_80190D80.messages, 8);
    state = &D_80190D80;
    thread1 = &state->producer;
    func_80027ED0(thread1, 0x1FE, func_8006B1DC, state, D_80191138 + sizeof(D_80191138), 0x5A);
    func_80032B50(thread1);
    thread2 = &state->consumer;
    func_80027ED0(thread2, 0x200, D_8006B334, state, D_80195138 + sizeof(D_80195138), 0x46);
    func_80032B50(thread2);
    end = commands;
    end = emit(end, 0xDE000000, (u32)D_13CA50);
    end = emit(end, 0xE9000000, 0);
    end = emit(end, 0xDF000000, 0);
    ++D_801D85D0.flags[3];
    func_8006B7F8(commands, (u8 *)end - (u8 *)commands, 0, 0,
        manager_buffers(&D_80190D80)[state->cursor].image, state->depthbuffer, 0, 0);
    func_8006CC80();
}
