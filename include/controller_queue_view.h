#ifndef SHIREN2_CONTROLLER_QUEUE_VIEW_H
#define SHIREN2_CONTROLLER_QUEUE_VIEW_H

/* Optional source-only declaration adaptation. These distinct names coexist
 * with common.h's signed-int s32. Underlying member types and member names
 * match both unchanged accepted anonymous ProbeMessageQueue definitions.
 * No storage or complete thread type is defined.
 */
typedef signed long ControllerQueueS32;
typedef void *ControllerQueueMessage;

struct ProbeThread;
typedef struct {
    struct ProbeThread *receive_waiters;
    struct ProbeThread *send_waiters;
    ControllerQueueS32 valid_count;
    ControllerQueueS32 first;
    ControllerQueueS32 capacity;
    ControllerQueueMessage *messages;
} ControllerQueueView;

#endif
