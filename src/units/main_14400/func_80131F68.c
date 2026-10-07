#include "common.h"
struct Thread;
typedef struct {
    struct Thread *receive_waiters;
    struct Thread *send_waiters;
    long valid_count;
    long first;
    long capacity;
    void **messages;
} Queue;
typedef struct { s32 field0; Queue *field4; s32 field8; s32 fieldC; } Message;
extern Queue D_801D9334;
extern void func_80027EA0(Queue *, void **, long);
extern long func_80031D50(Queue *, Message *, long);
extern long func_8002FEA0(Queue *, void **, long);
void func_80131F68(void) {
    Queue queue;
    Message message;
    void *storage;
    message.field0 = 0x7F00;
    message.fieldC = 0;
    message.field4 = &queue;
    func_80027EA0(&queue, &storage, 1);
    func_80031D50(&D_801D9334, &message, 1);
    func_8002FEA0(&queue, 0, 1);
}
