#include "common.h"
#include "controller_queue_view.h"

typedef unsigned short u16;
typedef unsigned long long u64;
typedef s32 OSPri;
typedef void *OSMesg;
typedef u64 OSTime;
typedef struct OSMesgQueue OSMesgQueue;

typedef struct OSThread {
    struct OSThread *next;
    OSPri priority;
    struct OSThread **queue;
    struct OSThread *tlnext;
    u16 state;
    u16 flags;
} OSThread;

extern OSThread *D_80037338; /* __osRunQueue */
extern OSThread *D_80037340; /* __osRunningThread */

u32 func_8002AF70(void);
void func_8002AFE0(u32);

typedef struct {
    OSMesgQueue *messageQueue;
    OSMesg message;
} __OSEventState;

extern __OSEventState D_800412D0[]; /* __osEventStateTab */
extern u32 D_8003725C;             /* __osShutdown */
extern u32 D_80037300;             /* __osPreNMI */

ControllerQueueS32 func_80031D50(OSMesgQueue *, OSMesg, ControllerQueueS32);

void func_80031E90(s32 event, OSMesgQueue *mq, OSMesg msg) {
    register u32 saveMask = func_8002AF70();
    __OSEventState *es;

    es = &D_800412D0[event];
    es->messageQueue = mq;
    es->message = msg;
    if (event == 14) {
        if (D_8003725C && !D_80037300) {
            func_80031D50(mq, msg, 0);
        }
        D_80037300 = 1;
    }
    func_8002AFE0(saveMask);
}
