#include "common.h"
#include "controller_queue_view.h"

/* Completion queue (initialized by func_80130A80). */
extern ControllerQueueView D_801E4E80;
extern ControllerQueueS32 func_8002FEA0(ControllerQueueView *queue, ControllerQueueMessage *message, ControllerQueueS32 flags);

/* Blocking receive with no message output; the status is intentionally ignored. */
void func_80130BA8(void) { func_8002FEA0(&D_801E4E80, 0, 1); }
