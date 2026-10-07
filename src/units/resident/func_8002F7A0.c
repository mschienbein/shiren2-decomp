#include "common.h"
#include "controller_queue_view.h"

extern s32 D_80037280;                 /* __osPiDevMgr.active */
extern ControllerQueueView *D_80037288; /* __osPiDevMgr.cmdQueue */

/* osPiGetCmdQueue: callers pass the result to osSendMesg/osJamMesg. */
ControllerQueueView *func_8002F7A0(void)
{
    if (D_80037280 == 0) {
        return 0;
    }
    return D_80037288;
}
