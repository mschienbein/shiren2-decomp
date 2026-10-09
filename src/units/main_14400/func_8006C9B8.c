#include "common.h"

/* One scheduler starts at 0x801D85D0. func_8006BFD0 constructs these seven
 * queues and five 0x1B0-byte threads, then passes this base to each thread.
 * Interior linker labels name members, never independent C storage. */
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
    s32 (*callback)(void);          /* +0x9EC */
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

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef SchedulerQueue MQ8006C9B8;
typedef struct { s32 type; void *task; } Msg8006C9B8;
typedef struct {
    u8 pad0;
    u8 flags;
    u8 pad2[0xE];
    u8 unk10[0x40];
    MQ8006C9B8 *replyQueue;
    void *replyMsg;
} Task8006C9B8;
typedef struct { s32 unk0; Task8006C9B8 *task; } Wrap8006C9B8;
typedef Scheduler Sched8006C9B8;
extern char D_8014C704[];
void func_80033048(const char *fmt, ...);
long func_8002FEA0(MQ8006C9B8 *mq, void *msg, long flags);
long func_80031D50(MQ8006C9B8 *mq, void *msg, long flags);
u32 func_80031F90(u32 mask);
void func_80032890(void *arg);
void func_80032A9C(void *arg);
void func_8006CBE8(Sched8006C9B8 *sc, Task8006C9B8 *task);
s32 func_8006D184(u8 id, u8 on);
void func_8006C9B8(Sched8006C9B8 *sc) {
    Msg8006C9B8 msgs[8];
    Wrap8006C9B8 *msg;
    Task8006C9B8 *task;
    u32 mask;
    u8 idx = 0;
    func_80033048(D_8014C704);
    for (;;) {
        func_8002FEA0(&sc->mq104, &msg, 1);
        task = msg->task;
        if (sc->state.flags9F7 & 2) {
            if (task->replyQueue != 0) {
                func_80031D50(task->replyQueue, task->replyMsg, 1);
            }
            continue;
        }
        func_8006CBE8(sc, task);
        mask = func_80031F90(1);
        if (sc->state.task9DC != 0) {
            sc->state.task9E8 = task;
            func_80031F90(mask);
            func_8002FEA0(&sc->mq12C, 0, 1);
            mask = func_80031F90(1);
            sc->state.task9E8 = 0;
        }
        func_80031F90(mask);
        mask = func_80031F90(1);
        func_8006D184(6, 0);
        func_8006D184(7, 0);
        func_80031F90(mask);
        mask = func_80031F90(1);
        sc->state.task9E0 = task;
        func_80031F90(mask);
        func_80032890(task->unk10);
        func_80032A9C(task->unk10);
        func_8002FEA0(&sc->mq40, 0, 1);
        mask = func_80031F90(1);
        sc->state.task9E0 = 0;
        func_80031F90(mask);
        func_8006D184(6, 1);
        mask = func_80031F90(1);
        if (sc->state.task9E4 != 0) {
            func_80031F90(mask);
            func_80031D50(&sc->mq12C, 0, 1);
            mask = func_80031F90(1);
        }
        func_80031F90(mask);
        if (!(task->flags & 2)) {
            func_8002FEA0(&sc->mq78, 0, 1);
        }
        func_8006D184(7, 1);
        if (sc->state.flags9F9 != 0) {
            sc->state.flags9F9--;
        }
        if (task->replyQueue != 0) {
            func_80031D50(task->replyQueue, task->replyMsg, 1);
        }
        if (task->flags & 1) {
            Msg8006C9B8 *m = &msgs[idx];
            m->type = 0xB;
            m->task = task;
            func_80031D50(&sc->mqB0, m, 1);
            idx = (idx + 1) % 8;
        }
    }
}
