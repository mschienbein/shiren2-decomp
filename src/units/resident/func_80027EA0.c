#include "message_queue_types.h"

void func_80027EA0(ProbeMessageQueue *queue, ProbeMessage *messages,
                   s32 capacity)
{
    struct ProbeThread *tail = (struct ProbeThread *)&D_80037330;

    queue->receive_waiters = tail;
    queue->send_waiters = tail;
    queue->valid_count = 0;
    queue->first = 0;
    queue->capacity = capacity;
    queue->messages = messages;
}
