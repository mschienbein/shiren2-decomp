#include "message_queue_types.h"

s32 func_8002FEA0(ProbeMessageQueue *queue, ProbeMessage *message, s32 flags)
{
    unsigned int saved_mask;
    s32 next_first;
    s32 remaining;
    struct ProbeThread *waiting_sender;

    saved_mask = func_8002AF70();
    while (queue->valid_count == 0) {
        if (flags == 0) {
            func_8002AFE0(saved_mask);
            return -1;
        }
        D_80037340->state = 8;
        func_8002A68C(&queue->receive_waiters);
    }

    if (message != 0) {
        *message = queue->messages[queue->first];
    }

    next_first = (s32)((u32)queue->first + 1UL) % queue->capacity;
    remaining = queue->valid_count;
    waiting_sender = queue->send_waiters;
    queue->valid_count = (s32)((u32)remaining - 1UL);
    queue->first = next_first;

    if (waiting_sender->next != 0) {
        func_80032B50(func_8002A7DC(&queue->send_waiters));
    }

    func_8002AFE0(saved_mask);
    return 0;
}
