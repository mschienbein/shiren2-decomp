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

typedef SchedulerClient Node8006C350;
extern Scheduler D_801D85D0;
u32 func_80031F90(u32 mask);
long func_80031D50(void *mq, void *msg, long flags);
void func_8006C350(Node8006C350 *node, void *mq, u8 flags) {
    SchedulerState *list;
    u32 saved = func_80031F90(1);
    list = &D_801D85D0.state;
    node->mq = mq;
    node->next = list->head;
    node->flags = flags;
    list->head = node;
    if ((flags & 2) && list->flags9F7) {
        func_80031D50(mq, &D_801D85D0.messages[1], 0);
    }
    func_80031F90(saved);
}
