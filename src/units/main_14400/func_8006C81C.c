#include "common.h"

typedef unsigned char u8;
typedef struct { u8 bytes[0x18]; } MessageQueue;
typedef struct { u8 bytes[0x40]; } TaskPayload;
typedef struct {
    u8 pad_00;
    u8 flags_01;
    u8 pad_02[0xE];
    TaskPayload payload_10;
    MessageQueue *reply_queue_50;
    void *reply_message_54;
} Task;
typedef struct { s32 type; Task *task; } Message;
typedef struct {
    u8 pad_00[0x40];
    MessageQueue completed_40;
    u8 pad_58[0x90];
    MessageQueue incoming_E8;
    u8 pad_100[0x2C];
    MessageQueue switch_12C;
    u8 pad_144[0x898];
    Task *current_9DC;
    Task *suspended_9E0;
    Task *waiting_9E4;
    Task *other_9E8;
    u8 pad_9EC[0xB];
    u8 flags_9F7;
} Scheduler;
extern const char D_8014C6DC[];
extern void func_80033048(const char *format, ...);
extern long func_8002FEA0(MessageQueue *queue, void *message, long flags);
extern long func_80031D50(MessageQueue *queue, void *message, long flags);
extern void func_800347A0(void);
extern void func_80032AD0(void);
extern u32 func_80032AF0(TaskPayload *task);
extern s32 func_8006D184(u8 id, u8 on);
extern void func_80032890(TaskPayload *task);
extern void func_80032A9C(TaskPayload *task);

/* Scheduler worker: receives tasks, pre-empts a suspended task when needed,
 * runs the task's payload and replies to its queue. */
void func_8006C81C(Scheduler *scheduler)
{
    Message *message;
    MessageQueue *completed;
    func_80033048(D_8014C6DC);
    completed = &scheduler->completed_40;
    for (;;) {
        Task *task;
        Task *suspended;
        TaskPayload *payload;
        u8 state;
        func_8002FEA0(&scheduler->incoming_E8, &message, 1);
        task = message->task;
        if (scheduler->flags_9F7 & 2) {
            if (task->reply_queue_50 != 0) {
                func_80031D50(task->reply_queue_50, task->reply_message_54, 1);
            }
            continue;
        }
        state = 0;
        func_800347A0();
        suspended = scheduler->suspended_9E0;
        if (suspended != 0) {
            if (suspended->flags_01 == 4) {
                scheduler->waiting_9E4 = task;
                func_8002FEA0(&scheduler->switch_12C, 0, 1);
                scheduler->waiting_9E4 = 0;
            } else {
                func_80032AD0();
                func_8002FEA0(&scheduler->completed_40, 0, 1);
                state = func_80032AF0(&suspended->payload_10) ? 1 : 2;
            }
        }
        func_8006D184(5, 0);
        payload = &task->payload_10;
        scheduler->current_9DC = task;
        func_80032890(payload);
        func_80032A9C(payload);
        func_8002FEA0(completed, 0, 1);
        scheduler->current_9DC = 0;
        func_8006D184(5, 1);
        if (scheduler->other_9E8 != 0) {
            func_80031D50(&scheduler->switch_12C, 0, 1);
        }
        if (state == 1) {
            payload = &suspended->payload_10;
            func_80032890(payload);
            func_80032A9C(payload);
        } else if (state == 2) {
            func_80031D50(completed, (void *)2, 1);
        }
        if (task->reply_queue_50 != 0) {
            func_80031D50(task->reply_queue_50, task->reply_message_54, 1);
        }
    }
}
