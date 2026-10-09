#include "common.h"
#include "controller_queue_view.h"

/* Completion queue (initialized by func_80130A80). */
extern ControllerQueueView D_801E4E80;
extern ControllerQueueS32 func_80031D50(ControllerQueueView *queue, ControllerQueueMessage message, ControllerQueueS32 flags);

void func_80130B80(void) {
    /* Null message, blocking send; the status is intentionally ignored. */
    func_80031D50(&D_801E4E80, 0, 1);
}
