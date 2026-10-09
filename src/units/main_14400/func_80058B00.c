#include "common.h"

typedef void *Message;
typedef struct Thread Thread;
typedef struct {
    Thread *receive_waiters;
    Thread *send_waiters;
    long valid_count;
    long first;
    long capacity;
    Message *messages;
} Queue;
extern Queue *D_801652EC[5];
extern u32 func_80031F90(u32 mask);
extern void func_80027EA0(Queue *queue, Message *messages, long capacity);
extern long func_8002FEA0(Queue *queue, Message *message, long flags);

void func_80058B00(void) {
    Queue queue;
    Message message;
    u32 mask;
    s32 i;
    mask = func_80031F90(1);
    func_80027EA0(&queue, &message, 1);
    for (i = 0; i < 5; i++) {
        if (D_801652EC[i] == 0) {
            break;
        }
    }
    if (i < 5) {
        D_801652EC[i] = &queue;
        func_80031F90(mask);
        func_8002FEA0(&queue, 0, 1);
    }
    func_80031F90(mask);
}
