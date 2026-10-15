#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef struct Thread Thread;
typedef struct {
    Thread *receive, *send;
    long valid, first, capacity;
    void **messages;
} Queue;
typedef struct Node {
    struct Node *next;
    void *queue;
    u8 flags;
} Node;
typedef struct {
    u8 pad_00[0xB0];
    Queue queue;
    u8 pad_C8[0x92E];
    u8 minimum_frames;
    u8 pad_9F7;
    u8 pending_frames;
    u8 pad_9F9[2];
    u8 wait_for_input;
} Context;
typedef struct { u8 pad_00[4]; void *framebuffer; } Payload;
typedef struct { s32 kind; Payload *payload; } Event;
extern const char D_8014C72C[];
const char D_8014C748[] = "OOSC : pending_flag ON , but num_pendingFb <= 0\n";
const char D_8014C77C[] = "THREAD except Vblank or RCP fault !!\n";
const char D_8014C7A4[] = "ERROR !!!!  done task over  in vblank.\n";
extern u8 D_8013D3E0, D_801A6C72;
extern void func_80033048(const char *fmt, ...);
extern void func_8006C350(Node *node, void *queue, u8 flags);
extern long func_8002FEA0(Queue *queue, void **message, long flags);
extern void func_80058BA8(u16 *buttons, u16 *pressed, u8 *x, u8 *y);
extern void func_80033CC0(u8 active);
extern u32 func_80031F90(u32 mask);
extern void func_800343C0(void *framebuffer);

void func_8006CDA8(Context *context) {
    Node subscription;
    void *buffers[4];
    void *message;
    Event *event;
    u16 buttons;
    s8 x, y;
    u8 warnings = 0;
    u8 ready, pending, count;
    u32 elapsed, mask, i, index;
    func_80033048(D_8014C72C);
    func_8006C350(&subscription, &context->queue, 1);
    count = pending = ready = 0;
    elapsed = 0;
    for (;;) {
        func_8002FEA0(&context->queue, &message, 1);
        event = message;
        switch (event->kind) {
        case 0:
            elapsed++;
            if (pending) {
                if (context->pending_frames == 0) {
                    func_80033048(D_8014C748);
                }
                context->pending_frames--;
                pending = 0;
            }
            if (count) {
                if (!context->wait_for_input) {
                    if (elapsed >= context->minimum_frames) {
                        ready = 1;
                    }
                } else {
                    func_80058BA8(&buttons, 0, (u8 *)&x, (u8 *)&y);
                    if ((buttons || x || y) && elapsed >= 5) {
                        ready = 1;
                    }
                }
            }
            if (!context->wait_for_input && elapsed >= 300) {
                if (warnings < 5) {
                    warnings++;
                    func_80033048(D_8014C77C);
                }
            } else {
                warnings = 0;
            }
            break;
        case 11:
            index = count;
            if (index >= 4) {
                func_80033048(D_8014C7A4);
            } else {
                count++;
                buffers[index] = event->payload->framebuffer;
                if (count == 1 && !pending) {
                    if (!context->wait_for_input) {
                        if (elapsed >= context->minimum_frames) {
                            ready = 1;
                        }
                    } else {
                        func_80058BA8(&buttons, 0, (u8 *)&x, (u8 *)&y);
                        if ((buttons || x || y) && elapsed >= 5) {
                            ready = 1;
                        }
                    }
                }
            }
            break;
        }
        if (ready) {
            if (D_8013D3E0) {
                D_8013D3E0 = 0;
                func_80033CC0(0);
            }
            mask = func_80031F90(1);
            func_800343C0(buffers[0]);
            D_801A6C72 = 1;
            func_80031F90(mask);
            for (i = 0; i < count - 1; i++) {
                buffers[i] = buffers[i + 1];
            }
            count--;
            ready = 0;
            pending = 1;
            elapsed = 0;
        }
    }
}

/* Independent setter at 0x8006D058 (its own catalogue entry after the boundary revision).
 * Complete reviewed scheduler at 0x801D85D0, identical to src/units/main_14400/func_8006D070.c. */
typedef struct SchedulerThread {
    unsigned long long opaque[0x1B0 / 8];
} SchedulerThread;
typedef struct {
    SchedulerThread *receive_waiters;
    SchedulerThread *send_waiters;
    s32 valid_count;
    s32 first;
    s32 capacity;
    void **messages;
} SchedulerQueue;
typedef struct SchedulerClient {
    struct SchedulerClient *next;
    SchedulerQueue *mq;
    unsigned char flags;
} SchedulerClient;
typedef struct {
    SchedulerClient *head;                   /* +0x9D8 */
    void *task9DC;
    void *task9E0;
    void *task9E4;
    void *task9E8;
    s32 (*callback)(void);                   /* +0x9EC */
    s32 retrace_count;                       /* +0x9F0 */
    unsigned char refresh_rate;              /* +0x9F4 */
    unsigned char retrace_interval;
    unsigned char flags9F6;
    unsigned char flags9F7;
    unsigned char flags9F8;
    unsigned char flags9F9;
    unsigned char flags9FA;
    unsigned char flags9FB;
} SchedulerState;
typedef struct {
    s32 messages[2];                         /* +0x000 */
    SchedulerQueue mq08; void *messages20[8];
    SchedulerQueue mq40; void *messages58[8];
    SchedulerQueue mq78; void *messages90[8];
    SchedulerQueue mqB0; void *messagesC8[8];
    SchedulerQueue mqE8; void *messages100[1];
    SchedulerQueue mq104; void *messages11C[4];
    SchedulerQueue mq12C; void *messages144[8];
    /* The thread alignment leaves +0x164..+0x167 unused. */
    SchedulerThread threads[5];              /* +0x168..+0x9D7 */
    SchedulerState state;                    /* +0x9D8..+0x9FB */
} Scheduler;
typedef char SchedulerQueueSize[(sizeof(SchedulerQueue) == 0x18) ? 1 : -1];
typedef char SchedulerSize[(sizeof(Scheduler) == 0xA00) ? 1 : -1];
extern Scheduler D_801D85D0;
u8 func_8006D058(s32 value) {
    u8 *state = &D_801D85D0.state.flags9FB; /* Context.wait_for_input */
    u8 old = *state;
    *state = value;
    return old;
}
