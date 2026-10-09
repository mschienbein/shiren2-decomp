#include "common.h"
#include "controller_queue_view.h"

/* Synchronous request header sent to the D_801D9334 server queue (dispatcher
 * func_80131CC0): command, reply queue, handler result, operation payload. */
typedef struct {
    s32 command;
    ControllerQueueView *reply_queue;
    s32 result;
    void *payload;
} Request;
extern ControllerQueueView D_801D9334;
extern void func_80027EA0(ControllerQueueView *queue, ControllerQueueMessage *messages, ControllerQueueS32 capacity);
extern ControllerQueueS32 func_80031D50(ControllerQueueView *queue, ControllerQueueMessage message, ControllerQueueS32 flags);
extern ControllerQueueS32 func_8002FEA0(ControllerQueueView *queue, ControllerQueueMessage *message, ControllerQueueS32 flags);
s32 func_80131F04(s32 command, void *payload) {
    ControllerQueueView queue;
    Request request;
    /* Eight-byte message backing; the capacity-1 reply queue uses slot 0. */
    ControllerQueueMessage message[2];
    request.command = command;
    request.payload = payload;
    request.reply_queue = &queue;
    func_80027EA0(&queue, message, 1);
    /* Queue statuses are intentionally ignored; the reply carries no message. */
    func_80031D50(&D_801D9334, &request, 1);
    func_8002FEA0(&queue, 0, 1);
    return request.result;
}
