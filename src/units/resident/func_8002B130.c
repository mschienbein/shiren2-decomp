#include "common.h"
#include "controller_queue_view.h"

typedef unsigned short u16;
typedef void *OSMesg;

typedef struct OSThread {
    struct OSThread *next;
    s32 priority;
    struct OSThread **queue;
    struct OSThread *tlnext;
    u16 state;
} OSThread;

typedef struct {
    OSThread *mtqueue;
    OSThread *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;

extern OSThread *D_80037340; /* __osRunningThread */
extern u32 func_8002AF70(void);
extern void func_8002AFE0(u32 mask);
extern void func_8002A68C(OSThread **queue);
extern OSThread *func_8002A7DC(OSThread **queue);
extern void func_80032B50(OSThread *thread);

/* osJamMesg; result and flag use the queue family's long-based contract. */
ControllerQueueS32 func_8002B130(OSMesgQueue *mq, OSMesg msg, ControllerQueueS32 flag)
{
    register u32 saveMask;

    saveMask = func_8002AF70();
    while (mq->validCount >= mq->msgCount) {
        if (flag == 1) {
            D_80037340->state = 8;
            func_8002A68C(&mq->fullqueue);
        } else {
            func_8002AFE0(saveMask);
            return -1;
        }
    }
    mq->first = (mq->first + mq->msgCount - 1) % mq->msgCount;
    mq->msg[mq->first] = msg;
    mq->validCount++;
    if (mq->mtqueue->next != 0) {
        func_80032B50(func_8002A7DC(&mq->mtqueue));
    }
    func_8002AFE0(saveMask);
    return 0;
}
