#include "common.h"
#include "controller_queue_view.h"

extern ControllerQueueS32 func_80031D50(ControllerQueueView *queue,
                                        ControllerQueueMessage message,
                                        ControllerQueueS32 flags);
extern ControllerQueueView D_800413A8;

void func_80032330(void)
{
    func_80031D50(&D_800413A8, 0, 0);
}
