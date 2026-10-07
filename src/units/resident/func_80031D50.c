#include "message_queue_types.h"

s32 func_80031D50(ProbeMessageQueue *queue, ProbeMessage message, s32 flags)
{
    unsigned int saved_mask;
    s32 last;
    s32 count;
    struct ProbeThread *waiting_receiver;

    saved_mask = func_8002AF70();
    while (queue->valid_count >= queue->capacity) {
        if (flags != 1) {
            func_8002AFE0(saved_mask);
            return -1;
        }
        D_80037340->state = 8;
        func_8002A68C(&queue->send_waiters);
    }

    last = (s32)((u32)queue->first + (u32)queue->valid_count) % queue->capacity;
    queue->messages[last] = message;
    count = queue->valid_count;
    waiting_receiver = queue->receive_waiters;
    queue->valid_count = (s32)((u32)count + 1UL);

    if (waiting_receiver->next != 0) {
        func_80032B50(func_8002A7DC(&queue->receive_waiters));
    }

    func_8002AFE0(saved_mask);
    return 0;
}
