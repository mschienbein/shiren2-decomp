#include "common.h"
#include "controller_queue_view.h"

typedef struct OSThread OSThread;
typedef struct PiHandleView PiHandleView;
typedef ControllerQueueView OSMesgQueue;
typedef struct {
    s32 active;
    OSThread *thread;
    OSMesgQueue *cmdQueue;
    OSMesgQueue *evtQueue;
    OSMesgQueue *acsQueue;
    s32 (*dma)(s32, u32, void *, u32);
    long (*edma)(PiHandleView *, long, unsigned long, void *, unsigned long);
} OSDevMgr;

extern OSDevMgr D_80037280;

/* osPiGetCmdQueue: callers pass the result to osSendMesg/osJamMesg. */
ControllerQueueView *func_8002F7A0(void)
{
    if (D_80037280.active == 0) {
        return 0;
    }
    return D_80037280.cmdQueue;
}
